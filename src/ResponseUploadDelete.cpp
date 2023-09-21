#include "Response.h"

#include "iostream"

//std::cout << "path: " << path << " : " <<  path[path.length() - 1] << std::endl;
bool Response::hasAccess(const std::string& filepath, std::fstream& filestr){
	const std::string& path = filepath;
    bool directory = 0;

    if (path[path.length() - 1] == '/')
        directory = 1;
	if (access (path.c_str(), F_OK) != 0){
		setStatusCode("404");
		return false; // doesnt exist
	}
	if (filestr.is_open()){
		setStatusCode("403");
		return false; //already open
	}
	if (access (path.c_str(), R_OK) != 0){
		setStatusCode("403");
		return false; // no read access
	}
	filestr.open(path);
	if (!filestr.is_open() && directory == 0){
		setStatusCode("404");
		return false; //not opened
	}
	if (filestr.peek() == std::ifstream::traits_type::eof() && directory == 0) {
		setStatusCode("404");
		return false; // empty file
	}
	return true;
}

//change to config variables later
bool	Response::checkMethod(std::string &str)
{	
	std::string array[3] = {"GET", "POST", "DELETE"};
	for (size_t i = 0; i < 3; i++){
		if (str == array[i])
			return true;
	}
	return false;
}

std::string uniqueFileName(std::string path, std::string file)
{
	int i = 1;
	std::string extension = "";
	std::string filename = path + file;
	std::string rawfile = file;
	size_t lastDot = file.rfind('.');
	if (lastDot != std::string::npos){
		extension	= file.substr(lastDot);
		rawfile		= file.substr(0, lastDot);
	}
	while (access (filename.c_str(), F_OK) == 0)
	{
		file = rawfile + std::to_string(i) + extension;
		filename = path + file;
		i++;
	}
	return filename;
}

std::string	Response::GetFilename(std::string line){
	std::string filename = "";
	std::string::size_type pos = line.find("filename=");
	if (pos != std::string::npos){
		pos += 10;
		std::string::size_type pos2 = line.find("\"", pos);
		if (pos2 != std::string::npos)
			filename = line.substr(pos, pos2 - pos);
	}
	else{
		pos = line.find("name=");
		if (pos != std::string::npos){
			pos += 5;
			std::string::size_type pos2 = line.find("\"", pos);
			if (pos2 != std::string::npos){
				filename = line.substr(pos, pos2-pos);
			}
		}
	}
	return filename;
}

// std::cout << "boundary: " << this->getContentType().substr(30) + "\r\n" << std::endl;
// std::cout << "unieknaam: " << uniqueName << std::endl;
void	Response::MakeFiles(std::stringstream &raw, std::string path, Settings &settings, const std::string &contentType)
{
	std::string boundary = "--" + contentType.substr(30) + "\r";
	std::string filename = "";
	std::string rawstring = raw.str();
	size_t bound		= rawstring.find(boundary);
	size_t start		= rawstring.find("\r\n\r\n", bound);
    size_t end			= rawstring.find("\r\n--" + contentType.substr(30), start);
	if (bound == std::string::npos || start == std::string::npos || end == std::string::npos)
		return ;
	start += 4;
	std::string bodystr = rawstring.substr(start, end - start);
	if (filename == "")
		filename = GetFilename(rawstring);
	if (filename != ""){
		std::string uniqueName = uniqueFileName(settings.getRoot() + path + "/", filename);
		std::ofstream file(uniqueName, std::ios::binary);
		file << bodystr;
		file.close();
	}
}

//std::cout << settings->getUploadEnable()        << std::endl;
//std::cout << settings->getClientMaxBodySize()   << std::endl;
//std::cout << settings->getAllowMethods()        << std::endl;
//std::cout << settings->getUploadPath()           << std::endl;
//std::cout << "Geupload" <<std::endl;
int Response::uploadFile(std::stringstream& raw, Settings *settings, const std::string &contentType){
    if (!settings->getUploadEnable()){
        setStatusCode("403");
        return 0;
    }
    if (access((settings->getRoot() +  settings->getUploadPath()).c_str(), W_OK) == -1) {
        setStatusCode("403");
        return 0;
    }
    if (raw.str().size() > settings->getClientMaxBodySize()){
        setStatusCode("413");
        return 0;
    }
    if (settings->getAllowMethods() < 2) {
        setStatusCode("405");
        return 0;
    }
	MakeFiles(raw, settings->getUploadPath(), *settings, contentType);
	return 1;
}

void	Response::deletePage(std::string path,  Settings *settings)
{
	setStatusCode("202");
	 if (settings->getAllowMethods() < 4){
	 	setStatusCode("405");
	 	return ;
	 }
	if (isDirectory(path)){
		if (path[path.length() - 1] != '/')
			path = path + '/';
		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir(path.c_str())) == NULL){
			setStatusCode("403");
			return ;
		}
		while ((ent = readdir(dir)) != NULL) {
			if (isDirectory(path + ent->d_name) && std::string(ent->d_name) != "." && std::string(ent->d_name) != "..")
                deletePage(path + ent->d_name, settings);
			if (std::string(ent->d_name) == "." || std::string(ent->d_name) == "..")
				continue;
			else if (remove((path + std::string(ent->d_name)).c_str()) != 0){
				setStatusCode("405");
		    }
        }
		closedir(dir);
	}
	if (remove(path.c_str()) != 0 ){
		setStatusCode("405");
		return ;
	}
	std::stringstream p;
	p << 	"<html>\n"
			"<body>\n"
			"<h1>File deleted.</h1>\n"
			"</body>\n"
			"</html>\n";
	setBody(p.str());
	setStatusCode("200");
}