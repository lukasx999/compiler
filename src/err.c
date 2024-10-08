#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>


#include "err.h"
#include "colors.h"



static void
print_error_header(Token tok, const char *filename, const char *message) {
    fprintf(stderr, "%s%s%s:%lu:%lu: %s", COLOR_BOLD, COLOR_WHITE, filename, tok.line_number, tok.column, COLOR_END);
    fprintf(stderr, "%s%sERROR: %s", COLOR_RED, COLOR_BOLD, COLOR_END);
    fprintf(stderr, "%s\n", message);
    fprintf(stderr, "%s   %lu %s|\t", COLOR_WHITE, tok.line_number, COLOR_END);
}


static size_t
get_offset(size_t pos, char *source) {
    size_t offset = 0;
    for (size_t i=pos ;; --i) {
        if (source[i] == '\n' || i==0) {
            offset = i==0 ? i : i+1; // +1 to get rid of newline at the beginning
            break;
        }
    }
  return offset;
}


void
err_correct(Token tok, const char *message, const char *correct, const char *source, const char *filename) {

    print_error_header(tok, filename, message);

    #if 0
    size_t pos = tok.absolute_pos - 1;
    size_t len = tok.length;
    char buf[BUFSIZE] = { 0 };

    size_t offset = get_offset(pos, source);

    strncpy(buf, source+offset, pos + len);
    fprintf(stderr, "%s", buf);

    fprintf(stderr, "%s%s%s", COLOR_GREEN, correct, COLOR_END);

    memset(buf, 0, BUFSIZE);
    strncpy(buf, source+pos+len, strcspn(source+pos+len, "\n"));
    fprintf(stderr, "%s\n\n", buf);
    #endif

}




void
err_throw(Token tok, const char *message, const char *source, const char *filename) {

    print_error_header(tok, filename, message);

    #if 0
    size_t pos = tok.absolute_pos - 1;
    size_t len = tok.length;
    char buf[BUFSIZE] = { 0 };

    size_t offset = get_offset(pos, source);

    strncpy(buf, source+offset, pos-offset);
    fprintf(stderr, "%s", buf);

    memset(buf, 0, BUFSIZE);
    strncpy(buf, source+pos, len),
    fprintf(stderr, "%s%s%s%s", COLOR_RED, COLOR_UNDERLINE, buf, COLOR_END);

    memset(buf, 0, BUFSIZE);
    strncpy(buf, source+pos+len, strcspn(source+pos+len, "\n"));
    fprintf(stderr, "%s\n\n", buf);
    #endif

}
