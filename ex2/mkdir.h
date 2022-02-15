/*
 * Student Name:        John Bradley
 * Student NetID:       jeb446
 * Compiler Used:       gcc
 * Program Description:
 *	program creates a directory structure for files associted with the exercises, assignments, and tests
 */

#ifndef MKDIR_3183
#define MKDIR_3183

/* all of the extra fluff that surrounds mkdir every time its used */
void mkdir_3183 (char *dirname);

/* the loops to create the subdirectories for each directory */
void mkstructure (char *base, int sub);

#endif
