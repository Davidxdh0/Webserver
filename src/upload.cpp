#include "Response.h"
#include "Path.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

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
bool	isUpload()
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
bool	checkMethods()
{	
	std::string method = "POST";
	std::string array[3] = {"GET", "POST", "DELETE"};
	for (size_t i = 0; i < 3; i++){
		if (method == array[i])
			return true;
	}
	return false;
}

// bool	fileExist(){

// }

// bool	fileOpen(){

// }

int uploadFile(){

	if (!isUpload())
		return 0;
	if (!checkMethods())
		return 0;
	// if (!fileExist())
	// 	return 0;
	// if (!fileOpen())
	// 	return 0;
	
	std::cout << "post" <<std::endl;
	return 1;
}

void	Response::upload(std::stringstream &requestRaw){
	// struct stat file_info;
	std::cout << "\nResponse::upload\n" << std::endl;
	std::cout << "Content of requestRaw:" << std::endl;
	requestRaw.str();
	std::string line;
	std::cout << _body << std::endl;
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
