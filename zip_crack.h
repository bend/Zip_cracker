#ifndef __ZIP_CRACK_H__
#define __ZIP_CRACK_H__

#define FILE_SIZE	12
#define CRC_SIZE	2
#define HEADER_SIZE	(FILE_SIZE+CRC_SIZE)

struct zip_archive {
    const char file_path[256];
    char buffer[HEADER_SIZE];
};

struct zip_archive * zip_load_archive(const char * file);
void zip_close_archive(struct zip_archive * archive);
int zip_test_password (struct zip_archive * archive, const char * pwd);

#endif
