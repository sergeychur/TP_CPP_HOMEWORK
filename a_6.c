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
#define IO_ERR -3
#define ERR_NO_LINES -4
#define ERR_CONVERT -5

typedef struct {
	char** line;
	size_t size;
	size_t capacity;
} str_vector;

int error_end(void) {
	// defines the behaviour after possible mistake in main
	fputs("[error]\n", stderr);
	return 0;
}

int push_back(str_vector* dst, char* src) {
	// place the strig nto the end aof thre array
	if(!src) {
		return ERR_EMPTY_PTR;
	}
	if(dst->size + 1 >= dst->capacity) {
		size_t new_capacity = (dst->capacity == 0) ? 1 : dst->capacity * 2;
		dst->line = (char**)realloc(dst->line, sizeof(char*) * new_capacity);
		if(!dst->line) {
			return ERR_ALLOC;
		}
		dst->capacity = new_capacity;
	}
	(dst->line)[dst->size] = src;
	++(dst->size);
	return 0;
}



void free_arr(str_vector* str_arr) {
	// free the recources
	if(str_arr->line) {
		for(size_t i = 0; i < str_arr->size; ++i) {
			if((str_arr->line)[i]) {
				free((str_arr->line)[i]);
			}
		}
		str_arr->capacity = 0;
		str_arr->size = 0;
		free(str_arr->line);
	}
}

int arr_print(const str_vector str_arr) {
	// printing the array of strings
	if(!(str_arr.line)) {
		return ERR_EMPTY_PTR;
	}
	for(size_t i = 0; i < str_arr.size; ++i) {
		if(!(str_arr.line)[i]) {
			return ERR_EMPTY_PTR;
		}
		puts((str_arr.line)[i]);
	}
	return 0;
}

int str_tolower(const char* src_line, char** dst_line) {
	// converting the string to lower case
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

int str_arr_tolower(const str_vector src_arr, const size_t str_num, str_vector* dst_arr) {
	// converting string array to lowercase
	if(!(src_arr.line)) {
		return ERR_EMPTY_PTR;
	}
	for(size_t i = 0; i < str_num; ++i) {
		if(!(src_arr.line)[i]) {
			return ERR_EMPTY_PTR;
		}
		char* line = (char*)malloc(sizeof(char) * (strlen(src_arr.line[i]) + 2));
		if(!line) {
			return ERR_ALLOC;
		}
		if(str_tolower((src_arr.line)[i], &line) != 0) {
			return ERR_EMPTY_PTR;
		}
		if(push_back(dst_arr, line) != 0) {
			free(line);
			return ERR_EMPTY_PTR;
		}
	}
	return 0;
}

char* input_string(void) {
	// reading the single line
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
	return string;
} 

int main(void) {
	// main
	str_vector str_arr = {NULL, 0, 0};
	size_t str_num = 0;
	while(!feof(stdin)) {
		char* line = input_string();
		if(!line) {
			if(feof(stdin)) {
				continue;
			}
			return error_end();
		}
		if(push_back(&str_arr, line) != 0) {
			free(line);
			free_arr(&str_arr);
			return error_end();
		}
		++str_num;
	}
	str_vector dst_arr = {NULL, 0, 0};
	if(str_arr_tolower(str_arr, str_num, &dst_arr) != 0) {
		free_arr(&str_arr);
		return error_end();
	}
	if(arr_print(dst_arr) != 0) {
		free_arr(&str_arr);
		free_arr(&dst_arr);
		return error_end();
	}
	free_arr(&str_arr);
	free_arr(&dst_arr);
	return 0;
}
