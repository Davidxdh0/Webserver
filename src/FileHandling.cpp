#include "Response.h"

#include "iostream"

bool Response::hasAccess(std::string filepath, std::fstream &filestr){

	if (access (filepath.c_str(), F_OK) != 0){
		setStatusCode("404");
		return 0; // doesnt exist
	}
	if (filestr.is_open()){
		setStatusCode("403");
		return 0; //already open
	}
	filestr.open(filepath);
	if (!filestr.is_open()){
		setStatusCode("404");
		return 0;; //not opened
	}
	if (access (filepath.c_str(), R_OK) != 0){
		setStatusCode("403");
		return 0; // no read access
	}
	char ch;
    if (!(filestr >> ch)){
		setStatusCode("404");
		return 0;//emptyfile
	}
	return 1;
}

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


bool	Response::isUpload()
{
	std::string path = "/Users/dyeboa/Documents/Webserv/public/upload";
    if (path.find("upload") != std::string::npos) {
        std::cout << "String contains upload" << std::endl;
        return true;
    }
    std::cout << "String does not contain upload" << std::endl;
    return false;
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



int Response::uploadFile(){

	if (!isUpload())
		return 0;
	// if (!checkMethod())
	// 	return 0;
	// if (!fileExist())
	// 	return 0;
	// if (!fileOpen())
	// 	return 0;
	
	std::cout << "post" <<std::endl;
	return 1;
}

void	Response::upload(){
	// struct stat file_info;

	std::cout << "\nResponse::upload\n" << std::endl;
	std::cout << "Content of requestRaw:" << std::endl;
	// getRequestRaw().str();
	std::string line;
	std::cout << getBody() << std::endl;
	std::cout << "Content of done:" << std::endl;
    
	// if (stat(file, &file_info) == 0) {
	// 	off_t size = file_info.st_size;
	// 	std::string filepath = std::string("/Users/dyeboa/Documents/Webserv/public/upload/filetoupload") + ".txt";
	// 	std::ofstream uploadfile(filepath);
	// 	size_t packetsize = 4096;
	// 	size_t dataSent = 0;
	// 	while (dataSent < static_cast<size_t>(size)){
	// 		size_t remaining = static_cast<size_t>(size) - dataSent;
	// 		size_t currentpacket = std::min(remaining, packetsize);
	// 		uploadfile <<  file + currentpacket;
	// 		dataSent += 4096;
	// 	}
	// }
	// else {
	// 	std::cout << "Uploading goes wrong" << std::endl;
	// }
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