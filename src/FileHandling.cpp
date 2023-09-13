#include "Response.h"

#include "iostream"

bool Response::hasAccess(const std::string& filepath, std::fstream& filestr){
	const std::string& path = filepath;
	if (access (path.c_str(), F_OK) != 0){
		setStatusCode("404");
		std::cout << "doesn't exist" << std::endl;
		return false; // doesnt exist
	}
	if (filestr.is_open()){
		setStatusCode("403");
		std::cout << "already open" << std::endl;
		return false; //already open
	}
	if (access (path.c_str(), R_OK) != 0){
		setStatusCode("403");
		std::cout << "no read access" << std::endl;
		return false; // no read access
	}
	filestr.open(path);
	if (!filestr.is_open()){
		setStatusCode("404");
		std::cout << "not opened directory?" << std::endl;
		return false; //not opened
	}
	if (filestr.peek() == std::ifstream::traits_type::eof()) {
		setStatusCode("404");
		std::cout << "empty file" << std::endl;
		return false; // empty file
	}
	return true;
}
// int hasAccess(std::string filepath, std::fstream *filestr){
	
// }

/*
isUpload()
checkMethods accepted
checkFile bestaat in dir
kan openen?
stream to string
uploadpath
write stream to file at uploadpath
if i can open file, return goed
anders fout.

*/

//change config file.


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
	// extensie eraf, i erbij, extensie erbij.
	std::string filename = path + file;
	while (access (filename.c_str(), F_OK) != 0)
	{
		filename = filename + std::to_string(i);
		i++;
	}
	return filename;
}

void	Response::MakeFiles(std::stringstream &raw, std::string path)
{
	std::string line;
	std::string boundary = this->getContentType().substr(30);
	std::cout << "boundary: " << this->getContentType().substr(30) << std::endl;
	while (line != "\n") {
        getline(raw, line);
        if (line.empty())
            break;
		std::cout << "line " << line << std::endl;
        std::string::size_type pos = line.find("boundary=");
        if (pos != std::string::npos) {
            std::string key = line.substr(1, pos);
			boundary = line.substr(pos + 2);
			std::cout << "boundary: " << boundary << std::endl;
			if (key == "Content-Type:"){
				boundary = line.substr(pos + 2);
				std::cout << boundary << std::endl;
				path = "1";
			}
        }
    }
	//find boundary
	//write naar
}

int Response::uploadFile(std::stringstream& raw, std::string path){
	// if (!isUpload(path))
	// 	return 0;
	// if (!checkMethod())
	// 	return 0;
	// MakeFiles(raw, path);
	// if (!fileExist())
	// 	return 0;
	// if (!FileSize)
	// 	return 0;
	// getboundary
	// multiple files

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