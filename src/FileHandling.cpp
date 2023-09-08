#include "Response.h"

#include "iostream"

bool Response::hasAccess(std::string filepath, std::fstream& filestr){
	std::string path = filepath;
	if (access (path.c_str(), F_OK) != 0){
		setStatusCode("404");
		std::cout << "doesn't exist" << std::endl;
		return 0; // doesnt exist
	}
	if (filestr.is_open()){
		setStatusCode("403");
		std::cout << "already open" << std::endl;
		return 0; //already open
	}
	if (access (path.c_str(), R_OK) != 0){
		setStatusCode("403");
		std::cout << "no read access" << std::endl;
		return 0; // no read access
	}
	filestr.open(path);
	if (!filestr.is_open()){
		setStatusCode("404");
		std::cout << "not opened or directory" << std::endl;
		return 0; //not opened
	}
	if (filestr.peek() == std::ifstream::traits_type::eof()) {
		setStatusCode("404");
		std::cout << "empty file" << std::endl;
		return 0; // empty file
	}
	return 1;
}

bool	Response::isUpload(std::string paths)
{
	std::string path = "/Users/dyeboa/Documents/Webserv/public/upload";
    if (path.find("upload") != std::string::npos) {
        std::cout << "String contains upload" << std::endl;
        return true;
    }
    std::cout << "String does not contain upload" << std::endl;
    return false;
	paths = "s";
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
				std::cout  << pos2 << "  filename2: " << filename << std::endl;
			}	
		}
	}
	std::cout << "3" << std::endl;
	return filename;
}

// std::cout << "boundary: " << this->getContentType().substr(30) + "\r\n" << std::endl;
void	Response::MakeFiles(std::stringstream &raw, std::string path)
{
	std::string boundary = "--" + this->getContentType().substr(30) + "\r";
	std::string filename = "";
	std::string rawstring = raw.str();

	size_t bound		= rawstring.find(boundary);
	size_t start		= rawstring.find("\r\n\r\n", bound);
    size_t end			= rawstring.find("\r\n--" + this->getContentType().substr(30), start);
	if (bound == std::string::npos || start == std::string::npos || end == std::string::npos){
		std::cout << "Error when uploading" << std::endl;
		return ;
	}
	start += 4;
	std::string bodystr = rawstring.substr(start, end - start);
	if (filename == "")
		filename = GetFilename(rawstring);
	if (filename != ""){
		std::string uniqueName = uniqueFileName("public/upload/", filename);
		// std::cout << "unieknaam: " << uniqueName << std::endl;
		std::ofstream file(uniqueName, std::ios::binary);
		std::ofstream file2("public/upload/" + filename + "2", std::ios::binary);
		file << bodystr;
		file2.write(bodystr.c_str(), bodystr.size());
		file.close();
	}
	std::cout << "Done upload " << filename << " BodySize " << bodystr.size() << std::endl;
	//PATH?!
	path = "";
}

int Response::uploadFile(std::stringstream& raw, std::string path){
	// if (!isUpload(path))
	// 	return 0;
	// if (!checkMethod())
	// 	return 0;
	MakeFiles(raw, path);

	std::cout << "Geupload" <<std::endl;
	return 1;
	path = "1";
	raw.str();
}

void	Response::upload(){
	// struct stat file_info;

	std::cout << "\nResponse::upload\n" << std::endl;
	std::cout << "Content of requestRaw:" << std::endl;
	// getRequestRaw().str();
	std::string line;
	std::cout << getBody() << std::endl;
	std::cout << "Content of done:" << std::endl;
    
}

void	Response::deletePage(std::string path)
{
	setStatusCode("202");
	// std::string del = "DELETE";
	// if (!checkMethod(del)){
	// 	setStatusCode() _statusCode = 405;
	// 	std::cout << "Delete method not found" << std::endl;
	// 	return ;
	// }
	static int i = 0;
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
			if (isDirectory(path + ent->d_name) && std::string(ent->d_name) != "." && std::string(ent->d_name) != ".."){
				deletePage(path + ent->d_name);
				i++;
				continue;
			}
			if (std::string(ent->d_name) == "." || std::string(ent->d_name) == ".."){
				continue ;
			}
			if (remove((path + std::string(ent->d_name)).c_str()) != 0){
				setStatusCode("405");
			}
		}
		closedir(dir);
	}
	if (remove(path.c_str()) != 0 ){
		setStatusCode("405");
		return ;
	}
	i = 0;
	sleep(2);
	mkdir("public/AA", 0777);
	std::ofstream MyFile("public/AA/first.txt");
	std::ofstream MyFile1("public/AA/second.css");
	std::ofstream MyFile2("public/AA/third.php");
	std::ofstream MyFile3("public/AA/fourth");
	std::ofstream MyFile4("public/AA/fifth.html");
	
	std::stringstream p;
	p << 	"<html>\n"
			"<body>\n"
			"<h1>File deleted.</h1>\n"
			"</body>\n"
			"</html>\n";
	setBody(p.str());
	setStatusCode("200");
}