#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <dirent.h>

int main() {
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir("/nfs/homes/maricard/Documents/cursus/webserver/miniserver/html_pages/")) != NULL)
	{
        while ((ent = readdir(dir)) != NULL)
		{
            if (ent->d_type == DT_REG)
			{
				std::string file_name = ent->d_name;
				if (file_name != "." && file_name != "..")
                	std::cout << ent->d_name << std::endl;
			}
        }
        closedir(dir);
    }
	else
	{
        std::perror("Error: 404 Not Found");
        return EXIT_FAILURE;
    }

    return 0;
}
