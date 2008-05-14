/** @file fs.h
 *  @brief Flash file system
 *
 * A flash-friendly file system for the NXT on-board flash memory.
 */

/* Copyright (C) 2008 the NxOS developers
 *
 * See AUTHORS for a full list of the developers.
 *
 * Redistribution of this file is permitted under
 * the terms of the GNU Public License (GPL) version 2.
 */

#ifndef __NXOS_BASE_FS_H__
#define __NXOS_BASE_FS_H__

#include "base/types.h"
#include "base/drivers/_efc.h"

/** @addtogroup kernel */
/*@{*/

/** @defgroup fs Flash file system */
/*@{*/

/** Maximum number of files that can be stored by the filesystem.
 * The lack of dynamic memory allocator makes this a hardcoded
 * limitation.
 */
#define FS_MAX_OPENED_FILES 8

/** Filename length, in U32s. */
#define FS_FILENAME_SIZE 8

/** Maximum allowed filename length (in bytes). */
#define FS_FILENAME_LENGTH (FS_FILENAME_SIZE * sizeof(U32))

/** File I/O operations buffer size. */
#define FS_BUF_SIZE (EFC_PAGE_WORDS * sizeof(U32))

/** File system errors. */
typedef enum {
  FS_ERR_NO_ERROR = 0,
  FS_ERR_NOT_FORMATTED,
  FS_ERR_FILE_NOT_FOUND,
  FS_ERR_TOO_MANY_OPENED_FILES,
  FS_ERR_INVALID_FD,
  FS_ERR_END_OF_FILE,
  FS_ERR_UNSUPPORTED_MODE,
  FS_ERR_CORRUPTED_FILE,
  FS_ERR_FLASH_ERROR,
  FS_ERR_NO_SPACE_LEFT_ON_DEVICE,
  FS_ERR_INCORRECT_POS,
} fs_err_t;

/** File permission modes. */
typedef enum {
  FS_PERM_READONLY,
  FS_PERM_READWRITE,
  FS_PERM_EXECUTABLE,
} fs_perm_t;

/** File opening modes. */
typedef enum {
  FS_FILE_MODE_OPEN,
  FS_FILE_MODE_APPEND,
  FS_FILE_MODE_CREATE,
} fs_file_mode_t;

typedef struct {
  union {
    U32 raw[EFC_PAGE_WORDS];
    U8 bytes[FS_BUF_SIZE];
  } data;
  U16 page;
  U8 pos;
} fs_buffer_t;

/** File description structure, read from the file's metadata
 * (FS_FILE_METADATA_SIZE bytes). */
typedef struct {
  /** Denotes fd usage. */
  bool used;

  /** The file name. */
  char name[FS_FILENAME_LENGTH];
 
  /** File origin page on the flash */
  U16 origin;
  
  /** Last page of the file */
  //U16 last_page;
 
  /** The file size. */
  size_t size;

  /** File permissions. */
  fs_perm_t perms;
  
  /** Read buffer. */
  fs_buffer_t rbuf;

  /** Write buffer. */
  fs_buffer_t wbuf;
} fs_file_t;

/** File descriptor type. */
typedef U8 fs_fd_t;

/** Initializes the file system.
 *
 * @return An @a fs_err_t error describing the outcome of the operation.
 */
fs_err_t nx_fs_init(void);

/** Open a file.
 *
 * @param name The name of the file to open.
 * @param mode The requested file mode.
 * @param fd A pointer to the file descriptor to use.
 */
fs_err_t nx_fs_open(char *name, fs_file_mode_t mode, fs_fd_t *fd);

/** Get the file size.
 *
 * @param fd The file descriptor.
 * @return The file size as a @a size_t.
 */
size_t nx_fs_get_filesize(fs_fd_t fd);

/** Read one byte from a file.
 *
 * @param fd The descriptor for the file to read from.
 * @param byte A pointer to a U32 to write the read byte from.
 */
fs_err_t nx_fs_read(fs_fd_t fd, U32 *byte);

/** Write one byte to a file.
 *
 * @param fd The descriptor for the file to write to.
 * @param byte The byte to write to the file.
 * @return An fs_err_t describing the outcome of the operation.
 */
fs_err_t nx_fs_write(fs_fd_t fd, U32 byte);

/** Flush a file's write buffer. */
fs_err_t nx_fs_flush(fs_fd_t fd);

/** Close a file. */
fs_err_t nx_fs_close(fs_fd_t fd);

/** Get file permissions.
 *
 * @return The current file permissions.
 */
fs_perm_t nx_fs_get_perms(fs_fd_t fd);

/** Set file permissions.
 *
 * @param fd The file descpriptor.
 * @param perms The new file permissions.
 */
fs_err_t nx_fs_set_perms(fs_fd_t fd, fs_perm_t perms);

/** Remove a file. */
fs_err_t nx_fs_unlink(fs_fd_t fd);

/*@}*/
/*@}*/

#endif /* __NXOS_BASE_FS_H__ */
