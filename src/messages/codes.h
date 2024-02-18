/*
** EPITECH PROJECT, 2024
** myftp
** File description:
** codes
*/

#pragma once

static const char MSG_200[] = "200 Command okay.\r\n";
static const char MSG_500[] = "500 Syntax error, command unrecognized.\r\n";
/* This may include errors such as command line too long. */
static const char MSG_501[] =
"501 Syntax error in parameters or arguments.\r\n";
static const char MSG_202[] =
"202 Command not implemented, superfluous at this site.\r\n";
static const char MSG_502[] = "502 Command not implemented.\r\n";
static const char MSG_503[] = "503 Bad sequence of commands.\r\n";
static const char MSG_504[] =
"504 Command not implemented for that parameter.\r\n";

static const char MSG_110[] = "110 Restart marker reply.\r\n";
/* In this case, the text is exact and not left to the */
/* particular implementation; it must read: */
/*      MARK yyyy = mmmm */
/* Where yyyy is User-process data stream marker, and mmmm */
/* server's equivalent marker (note the spaces between markers */
/* and "="). */
static const char MSG_211[] = "211 System status, or system help reply.\r\n";
static const char MSG_212[] = "212 Directory status.\r\n";
static const char MSG_213[] = "213 File status.\r\n";
static const char MSG_214[] = "214 %s.\r\n";
/* On how to use the server or the meaning of a particular */
/* non-standard command.  This reply is useful only to the */
/* human user. */
static const char MSG_215[] = "215 %s system type.\r\n";
/* Where NAME is an official system name from the list in the */
/* Assigned Numbers document. */

static const char MSG_120[] = "120 Service ready in %d minutes.\r\n";
static const char MSG_220[] = "220 Service ready for new user.\r\n";
static const char MSG_221[] = "221 Service closing control connection.\r\n";
/* Logged out if appropriate. */
static const char MSG_421[] =
"421 Service not available, closing control connection.\r\n";
/* This may be a reply to any command if the service knows it */
/* must shut down. */
static const char MSG_125[] =
"125 Data connection already open; transfer starting.\r\n";
static const char MSG_225[] =
"225 Data connection open; no transfer in progress.\r\n";
static const char MSG_425[] = "425 Can't open data connection.\r\n";
static const char MSG_226[] = "226 Closing data connection.\r\n";
/* Requested file action successful (for example, file */
/* transfer or file abort). */
static const char MSG_426[] = "426 Connection closed; transfer aborted.\r\n";
static const char MSG_227[] =
"227 Entering Passive Mode (%d,%d,%d,%d,%d,%d).\r\n";

static const char MSG_230[] = "230 User logged in, proceed.\r\n";
static const char MSG_530[] = "530 Not logged in.\r\n";
static const char MSG_331[] = "331 User name okay, need password.\r\n";
static const char MSG_332[] = "332 Need account for login.\r\n";
static const char MSG_532[] = "532 Need account for storing files.\r\n";

static const char MSG_150[] =
"150 File status okay; about to open data connection.\r\n";
static const char MSG_250[] = "250 Requested file action okay, completed.\r\n";
static const char MSG_257[] = "257 \"%s\" created.\r\n";
static const char MSG_257_CD[] = "257 %s\r\n";
static const char MSG_350[] =
"350 Requested file action pending further information.\r\n";
static const char MSG_450[] = "450 Requested file action not taken.\r\n";
/* File unavailable (e.g., file busy). */
static const char MSG_550[] = "550 Requested action not taken.\r\n";
/* File unavailable (e.g., file not found, no access). */
static const char MSG_451[] =
"451 Requested action aborted. Local error in processing.\r\n";
static const char MSG_551[] =
"551 Requested action aborted. Page type unknown.\r\n";
static const char MSG_452[] = "452 Requested action not taken.\r\n";
/* Insufficient storage space in system. */
static const char MSG_552[] = "552 Requested file action aborted.\r\n";
/* Exceeded storage allocation (for current directory or */
/* dataset). */
static const char MSG_553[] = "553 Requested action not taken.\r\n";
/* File name not allowed. */
