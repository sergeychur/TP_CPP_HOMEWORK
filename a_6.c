/*
Составить программу построчной обработки текста. Суть обработки - приведение каждого символа в каждой строке к нижнему регистру.

Программа считывает входные данные со стандартного ввода, и печатает результат в стандартный вывод.

Процедура приведения к нижнему регистру должна быть оформлена в виде отдельной функции, которой на вход подается массив строк (который необходимо обработать), количество переданных строк, а также указатель на переменную, в которой необходимо разместить результат - массив уже обработанных строк. 
В качестве возвращаемого значения функция должна возвращать количество строк, содержащихся в результирующем массиве. 

Программа должна уметь обрабатывать ошибки - такие как неверные входные данные(отсутствие входных строк) или ошибки выделения памяти и т.п. 
В случае возникновения ошибки нужно выводить об этом сообщение "[error]" и завершать выполнение программы. 

*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ERR_EMPTY_PTR -1
#define ERR_ALLOC -2

#define SUCCESS 0

typedef struct {
	char** line_array;
	size_t size;
	size_t capacity;
} str_vector;

int error_end(void);
int copy_pointers(char**, char**, const size_t);
int push_back(str_vector*, char*);
void free_arr(str_vector*);
int arr_print(const str_vector);
int str_tolower(const char*, char**);
int str_arr_tolower(const str_vector, const size_t, str_vector*);
char* input_string(int*);
int fill_src_array(str_vector*, size_t*);

// main
int main(void) {
	str_vector str_arr = {NULL, 0, 0};
	size_t str_num = 0;
	int err_state = SUCCESS;
	err_state += fill_src_array(&str_arr, &str_num);
	str_vector dst_arr = {NULL, 0, 0};
	err_state += (err_state == SUCCESS) ? str_arr_tolower(str_arr, str_num, &dst_arr) : 0;
	free_arr(&str_arr);
	err_state += (err_state == SUCCESS) ? arr_print(dst_arr) : 0;
	free_arr(&dst_arr);
	return (err_state == 0) ? 0 : error_end();
}

// defines the behaviour after possible mistake in main
int error_end(void) {
	fputs("[error]", stderr);
	return 0;
}

// copying the array of pointers
int copy_pointers(char** dst, char** src, const size_t src_size) {
	if(!src && !dst) {
		return ERR_EMPTY_PTR;
	}
	for(size_t i = 0; i < src_size; ++i) {
		dst[i] = src[i];
	}
	return 0;
}

// place the strig nto the end aof thre array
int push_back(str_vector* dst, char* src) {
	if(!src) {
		return ERR_EMPTY_PTR;
	}
	if(dst->size + 1 >= dst->capacity) {
		size_t new_capacity = (dst->capacity == 0) ? 1 : dst->capacity * 2;
		char** tmp = (char**)malloc(sizeof(char*) * new_capacity);
		if(!tmp) {
			return ERR_ALLOC;
		}
		if(dst->line_array) {
			copy_pointers(tmp, dst->line_array, dst->size);
			free(dst->line_array);
		}
		dst->line_array = tmp;
		dst->capacity = new_capacity;
	}
	(dst->line_array)[dst->size] = src;
	++(dst->size);
	return 0;
}


// free the recources
void free_arr(str_vector* str_arr) {
	if(str_arr->line_array) {
		for(size_t i = 0; i < str_arr->size; ++i) {
			if((str_arr->line_array)[i]) {
				free((str_arr->line_array)[i]);
			}
		}
		str_arr->capacity = 0;
		str_arr->size = 0;
		free(str_arr->line_array);
	}
}

// printing the array of strings
int arr_print(const str_vector str_arr) {
	if(!(str_arr.line_array)) {
		return ERR_EMPTY_PTR;
	}
	for(size_t i = 0; i < str_arr.size; ++i) {
		if(!(str_arr.line_array)[i]) {
			return ERR_EMPTY_PTR;
		}
		puts((str_arr.line_array)[i]);
	}
	return 0;
}

// converting the string to lower case
int str_tolower(const char* src_line, char** dst_line) {
	if(!src_line) {
		return ERR_EMPTY_PTR;
	}
	size_t i = 0;
	for(i = 0; i < strlen(src_line); ++i) {
		(*dst_line)[i] = tolower(src_line[i]);
	}
	(*dst_line)[strlen(src_line)] = '\0';
	return 0;
}

// converting string array to lowercase
int str_arr_tolower(const str_vector src_arr, const size_t str_num, str_vector* dst_arr) {
	if(!(src_arr.line_array)) {
		return ERR_EMPTY_PTR;
	}
	for(size_t i = 0; i < str_num; ++i) {
		if(!(src_arr.line_array)[i]) {
			return ERR_EMPTY_PTR;
		}
		char* line = (char*)malloc(sizeof(char) * (strlen(src_arr.line_array[i]) + 2));
		if(!line) {
			return ERR_ALLOC;
		}
		if(str_tolower((src_arr.line_array)[i], &line) != SUCCESS) {
			return ERR_EMPTY_PTR;
		}
		if(push_back(dst_arr, line) != SUCCESS) {
			free(line);
			return ERR_EMPTY_PTR;
		}
	}
	return 0;
}

// reading the single line_array
char* input_string(int* err_state) {
	char* string = NULL;
	size_t size = 0;
	size_t capacity = 0;
	char c = '\0';
	while(c = getchar(), c != '\n' && c != EOF) {
		if(size + 1 >= capacity) {
			size_t new_capacity = (capacity == 0) ? 1 : capacity * 2;
			char* tmp = (char*)malloc((new_capacity + 1) * sizeof(char));
			if(!tmp) {
				if(string) {
					free(string);
				}
				*err_state += ERR_ALLOC;
				return NULL;
			}
			if(string) {
				tmp = strcpy(tmp, string);
				free(string);
			}
			string = tmp;
			capacity = new_capacity;
		}
		string[size] = c;
		string[size + 1] = '\0';
		++size;
	}
	if(size == 0) {
		string = (char*)malloc(2 * sizeof(char));
		string[0] = '\0';
	}
	return string;
}

// filling the array to work with later
int fill_src_array(str_vector* str_arr, size_t* str_num) {
	int err_state = SUCCESS;
	while(!feof(stdin) && !err_state) {
		char* line = input_string(&err_state);
		err_state += (err_state == SUCCESS) ? push_back(str_arr, line) : 0;
		if(line && err_state) {
			free(line);
		}
		++(*str_num);
	}
	return err_state;
}