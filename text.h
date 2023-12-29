// count letters from src (utf8)
size_t letters(char *src);
// count bytes used by src including null terminator.
size_t bytes(char *src);
// get utf8 letter, and set the pointer to the following
// letter in next. if there are no more letters, next
// won't be updated and the result will be 0.
unsigned int letter(char *src, char **next);
// check if src begins with n bytes from what.
int begins_withn(char *src, char *what, size_t n);
int begins_with(char *src, char *what);
// check if src ends with n bytes from what.
int ends_withn(char *src, char *what, size_t n);
int ends_with(char *src, char *what);
// check if src contains what.
int contains(char *src, char *what);
// find what in src. null is returned if no match is found.
char *find(char *src, char *what);
int find_index(char *src, char *what);
// find last match of what in src. null is returned
// if no match is found.
char *find_last(char *src, char *what);
int find_last_index(char *src, char *what);
// get pointer from src + n. n can be negative,
// for example, "lorem", -3, a pointer to "rem"
// will be returned.
char *from(char *src, int n);
// insert max bytes from what into src at index n.
void insertn(char *src, char *what, size_t max, int n);
// insert what at index n into src.
void insert(char *src, char *what, int n);
// erase bytes amount from src at index n.
void erase_bytes(char *src, int n, size_t bytes);
// erase what from src.
void erase(char *src, char *what);
// erase only first match of what from src.
void erase_first(char *src, char *what);
// erase only last match of what from src.
void erase_last(char *src, char *what);
// replace original from src with replacement.
void replace(char *src, char *original, char *replacement);
// replace only first match of original with replacement from src.
void replace_first(char *src, char *original, char *replacement);
// replace only last match of original with replacement from src.
void replace_last(char *src, char *original, char *replacement);
// check if src is an empty string.
int empty(char *src);
// append n bytes from what to src.
void appendn(char *src, char *what, size_t n);
// append what to src.
void append(char *src, char *what);
// insert n bytes from what at the beginning of src.
void prependn(char *src, char *what, size_t n);
// inser what at the beginning of src.
void prepend(char *src, char *what);
// remove beginning and ending white space from src.
void trim(char *src);
// update src to be max bytes from index n.
void substr(char *src, int n, size_t max);
// repeat what times at the index n from src.
void repeat(char *src, char *what, int n, size_t times);
