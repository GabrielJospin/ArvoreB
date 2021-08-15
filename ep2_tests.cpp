//--------------------------------------------------------------
// Testes - EP1 de AED2
//--------------------------------------------------------------
#include "Ep2.cpp"

typedef struct node {
  int key;
  struct node *next;
} NODE;

void print_list(NODE* node) {
  NODE* pointer = node;
  while(pointer) {
    printf("%d ", pointer->key);
    pointer = pointer->next;
  }
}

NODE* last_elem(NODE* node) {
  NODE* pointer = node;
  if (pointer) 
    while (pointer->next) 
      pointer = pointer->next;
  return pointer;
}

NODE* create_node(int key) {
  NODE* node = (NODE*) malloc(sizeof(NODE));
  node->key = key;
  node->next = NULL;
  return node;
}

void insert_elem(NODE* *node, int key) {
  NODE* last_node = last_elem(*node);
  NODE* new_node = create_node(key);
  if(!last_node) 
    *node = new_node;
  else last_node->next = new_node;
}

void insert_page(PAGINA page, NODE* *node, int* i) {
  int j;
  for (j = 1; j <= page.cont; j++) {
    insert_elem(node, page.item[j].chave);
    *i += 1;
  }
}

NODE* fill_list(int array[], int n) {
  int i;
  NODE* node = NULL;
  for (i = 0; i < n; i++)
    insert_elem(&node, array[i]);
  return node;
}

void delete_file(char* file_path) {
	FILE* file = fopen(file_path, "wb");
	fclose(file);
}

PAGINA read_page(FILE* file, int page_number) {
  PAGINA page;
  fseek(file, page_number*sizeof(PAGINA), SEEK_SET);
  fread(&page, sizeof(PAGINA), 1, file);
  return page;
}

bool is_equal(NODE* expected, NODE* result) {
  NODE* expected_pointer = expected;
  NODE* result_pointer = result;
  while(expected_pointer && result_pointer) {
    if (expected_pointer->key != result_pointer->key) {
      return false;
    }
    expected_pointer = expected_pointer->next;
    result_pointer = result_pointer->next;
  }
  if (result_pointer || expected_pointer) 
    return false;
  return true;
}

void in_order(FILE* file, int root, NODE* *node, int* i) {
  int j;
  if (root == -1) return;
  PAGINA page = read_page(file,root);
  in_order(file, page.item[0].linkdir, node, i);
  insert_page(page, node, i);
  in_order(file, page.item[1].linkdir, node, i);
  in_order(file, page.item[2].linkdir, node, i);
}

void write_list(FILE* file, int x, int y, NODE* node) {
	if(!node) {
		fprintf(file,"NULL\n");
		return;
	}
	NODE* pointer = node;
	while(pointer) {
		fprintf(file,"%i ",pointer->key);
		pointer = pointer->next;
	}
	fprintf(file,"\n");
}

void write_error(FILE* log, NODE* expected, NODE* result) {
  fprintf(log, "Esperado:\n");
  // print_list(expected);
  write_list(log, 0, 0, expected);
  fprintf(log, "Obtido:\n");
  write_list(log, 0, 0, result);
  // print_list(result);
  fprintf(log, "\n");
}

void unit_test(char file_path[], FILE* log, int* errors, int* test_number, int root, int expected_array[], int n) {
    FILE* file = fopen(file_path, "rb");
  if (file) {
    printf("TESTE %d: ", *test_number);
    int i = 0;
    NODE* expected = fill_list(expected_array, n);
    NODE* result = NULL;
    in_order(file, root, &result, &i);
    if (is_equal(expected, result)) {
      printf("ok\n\n");
    } else {
      fprintf(log, "\nTESTE %d: erro\n", *test_number);
      write_error(log, expected, result);
      printf("erro\n\n");
      *errors += 1;
    }
    *test_number += 1;
    fclose(file);
  } else {
    printf("\nOcorreu algum erro ao abrir o arquivo...\n");
  }
}

void right_insertion_test(char file_path[], FILE* log, int* root, int* errors, int* test_number) {
  int length;
  *root = 0;

  int test_1[] = {1,2,3,4,8,5,6,7,9};
  length = 9;


	inserir(file_path, root, 1);
	inserir(file_path, root, 2);
	inserir(file_path, root, 3);
	inserir(file_path, root, 4);
	inserir(file_path, root, 5);
	inserir(file_path, root, 6);
	inserir(file_path, root, 7);
	inserir(file_path, root, 8);
    inserir(file_path, root, 9);

  unit_test(file_path, log, errors, test_number, *root, test_1, length);

  int test_2[] = {1,2,3,4,8,5,6,7,9,10,11};
  length = 11;
	inserir(file_path, root, 10);
	inserir(file_path, root, 11);
  unit_test(file_path, log, errors, test_number, *root, test_2, length);

  int test_3[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  length = 15;
	inserir(file_path, root, 12);
	inserir(file_path, root, 13);
	inserir(file_path, root, 14);
	inserir(file_path, root, 15);
  unit_test(file_path, log, errors, test_number, *root, test_3, length);

  int test_4[] = {1,2,3,4,5,6,7,8,9,10,11,12,16,13,14,15,17,18,19,20};
  length = 20;
	inserir(file_path, root, 16);
	inserir(file_path, root, 17);
	inserir(file_path, root, 18);
	inserir(file_path, root, 19);
	inserir(file_path, root, 20);
  unit_test(file_path, log, errors, test_number, *root, test_4, length);

  int test_5[] = {1,2,3,4,5,6,7,8,16,9,10,11,12,13,14,15,17,18,19,20,24,21,22,23,25,26,27};
  length = 27;
	inserir(file_path, root, 21);
	inserir(file_path, root, 22);
	inserir(file_path, root, 23);
	inserir(file_path, root, 24);
	inserir(file_path, root, 25);
	inserir(file_path, root, 26);
	inserir(file_path, root, 27);
  unit_test(file_path, log, errors, test_number, *root, test_5, length);

  int test_6[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,32,29,30,31,33,34,35,36};
  length = 36;
	inserir(file_path, root, 28);
	inserir(file_path, root, 29);
	inserir(file_path, root, 30);
	inserir(file_path, root, 31);
	inserir(file_path, root, 32);
	inserir(file_path, root, 33);
	inserir(file_path, root, 34);
	inserir(file_path, root, 35);
	inserir(file_path, root, 36);
  unit_test(file_path, log, errors, test_number, *root, test_6, length);

  int test_7[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,32,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,33,34,35,36,37,38,39,40,41,42,43,44,45,46,48,47,49,50};
  length = 50;
	inserir(file_path, root, 37);
	inserir(file_path, root, 38);
	inserir(file_path, root, 39);
	inserir(file_path, root, 40);
	inserir(file_path, root, 41);
	inserir(file_path, root, 42);
	inserir(file_path, root, 43);
	inserir(file_path, root, 44);
	inserir(file_path, root, 45);
	inserir(file_path, root, 46);
	inserir(file_path, root, 47);
	inserir(file_path, root, 48);
	inserir(file_path, root, 49);
	inserir(file_path, root, 50);
  unit_test(file_path, log, errors, test_number, *root, test_7, length);

  int test_8[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63};
  length = 63;
	inserir(file_path, root, 51);
	inserir(file_path, root, 52);
	inserir(file_path, root, 53);
	inserir(file_path, root, 54);
	inserir(file_path, root, 55);
	inserir(file_path, root, 56);
	inserir(file_path, root, 57);
	inserir(file_path, root, 58);
	inserir(file_path, root, 59);
	inserir(file_path, root, 60);
	inserir(file_path, root, 61);
	inserir(file_path, root, 62);
	inserir(file_path, root, 63);
  unit_test(file_path, log, errors, test_number, *root, test_8, length);

  int test_9[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,64,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,96,93,94,95,97,98,99,100};
  length = 100;
  inserir(file_path, root, 64);
	inserir(file_path, root, 65);
	inserir(file_path, root, 66);
	inserir(file_path, root, 67);
	inserir(file_path, root, 68);
	inserir(file_path, root, 69);
	inserir(file_path, root, 70);
	inserir(file_path, root, 71);
	inserir(file_path, root, 72);
	inserir(file_path, root, 73);
	inserir(file_path, root, 74);
	inserir(file_path, root, 75);
	inserir(file_path, root, 76);
  inserir(file_path, root, 77);
	inserir(file_path, root, 78);
	inserir(file_path, root, 79);
	inserir(file_path, root, 80);
	inserir(file_path, root, 81);
	inserir(file_path, root, 82);
	inserir(file_path, root, 83);
	inserir(file_path, root, 84);
	inserir(file_path, root, 85);
	inserir(file_path, root, 86);
	inserir(file_path, root, 87);
	inserir(file_path, root, 88);
	inserir(file_path, root, 89);
  inserir(file_path, root, 90);
	inserir(file_path, root, 91);
	inserir(file_path, root, 92);
	inserir(file_path, root, 93);
	inserir(file_path, root, 94);
	inserir(file_path, root, 95);
	inserir(file_path, root, 96);
	inserir(file_path, root, 97);
	inserir(file_path, root, 98);
	inserir(file_path, root, 99);
	inserir(file_path, root, 100);
  unit_test(file_path, log, errors, test_number, *root, test_9, length);

  delete_file(file_path);
}

void left_insertion_test(char file_path[], FILE* log, int* root, int* errors, int* test_number) {
  int length;
  *root = 0;

  int test_1[] = {98,99,100};
  length = 3;
	inserir(file_path, root, 100);
	inserir(file_path, root, 99);
	inserir(file_path, root, 98);
  unit_test(file_path, log, errors, test_number, *root, test_1, length);

  int test_2[] = {90,91,92,93,97,94,95,96,98,99,100};
  length = 11;
	inserir(file_path, root, 97);
	inserir(file_path, root, 96);
	inserir(file_path, root, 95);
	inserir(file_path, root, 94);
	inserir(file_path, root, 93);
	inserir(file_path, root, 92);
	inserir(file_path, root, 91);
  inserir(file_path, root, 90);
  unit_test(file_path, log, errors, test_number, *root, test_2, length);

  int test_3[] = {76,77,79,78,80,81,82,83,84,85,93,86,87,88,89,90,91,92,94,95,96,97,98,99,100};
  length = 25;
	inserir(file_path, root, 89);
	inserir(file_path, root, 88);
	inserir(file_path, root, 87);
	inserir(file_path, root, 86);
	inserir(file_path, root, 85);
	inserir(file_path, root, 84);
	inserir(file_path, root, 83);
  inserir(file_path, root, 82);
  inserir(file_path, root, 81);
  inserir(file_path, root, 80);
  inserir(file_path, root, 79);
  inserir(file_path, root, 78);
  inserir(file_path, root, 77);
  inserir(file_path, root, 76);
  unit_test(file_path, log, errors, test_number, *root, test_3, length);

  int test_4[] = {60,61,63,62,64,65,66,67,68,69,77,70,71,72,73,74,75,76,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100};
  length = 41;
	inserir(file_path, root, 75);
	inserir(file_path, root, 74);
	inserir(file_path, root, 73);
	inserir(file_path, root, 72);
	inserir(file_path, root, 71);
	inserir(file_path, root, 70);
	inserir(file_path, root, 69);
  inserir(file_path, root, 68);
  inserir(file_path, root, 67);
  inserir(file_path, root, 66);
  inserir(file_path, root, 65);
  inserir(file_path, root, 64);
  inserir(file_path, root, 63);
  inserir(file_path, root, 62);
  inserir(file_path, root, 61);
  inserir(file_path, root, 60);
  unit_test(file_path, log, errors, test_number, *root, test_4, length);

  int test_5[] = {30,31,32,33,34,35,36,37,45,38,39,40,41,42,43,44,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100};
  length = 71;
	inserir(file_path, root, 59);
	inserir(file_path, root, 58);
	inserir(file_path, root, 57);
	inserir(file_path, root, 56);
	inserir(file_path, root, 55);
	inserir(file_path, root, 54);
	inserir(file_path, root, 53);
  inserir(file_path, root, 52);
  inserir(file_path, root, 51);
  inserir(file_path, root, 50);
  inserir(file_path, root, 49);
  inserir(file_path, root, 48);
  inserir(file_path, root, 47);
  inserir(file_path, root, 46);
  inserir(file_path, root, 45);
  inserir(file_path, root, 44);
  inserir(file_path, root, 43);
  inserir(file_path, root, 42);
  inserir(file_path, root, 41);
  inserir(file_path, root, 40);
  inserir(file_path, root, 39);
  inserir(file_path, root, 38);
  inserir(file_path, root, 37);
  inserir(file_path, root, 36);
  inserir(file_path, root, 35);
  inserir(file_path, root, 34);
  inserir(file_path, root, 33);
  inserir(file_path, root, 32);
  inserir(file_path, root, 31);
  inserir(file_path, root, 30);
  unit_test(file_path, log, errors, test_number, *root, test_5, length);

  int test_6[] = {1,2,3,4,5,9,6,7,8,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,69,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100};
  length = 100;
	inserir(file_path, root, 29);
	inserir(file_path, root, 28);
	inserir(file_path, root, 27);
	inserir(file_path, root, 26);
	inserir(file_path, root, 25);
	inserir(file_path, root, 24);
	inserir(file_path, root, 23);
  inserir(file_path, root, 22);
  inserir(file_path, root, 21);
  inserir(file_path, root, 20);
  inserir(file_path, root, 19);
  inserir(file_path, root, 18);
  inserir(file_path, root, 17);
  inserir(file_path, root, 16);
  inserir(file_path, root, 15);
  inserir(file_path, root, 14);
  inserir(file_path, root, 13);
  inserir(file_path, root, 12);
  inserir(file_path, root, 11);
  inserir(file_path, root, 10);
  inserir(file_path, root, 9);
  inserir(file_path, root, 8);
  inserir(file_path, root, 7);
  inserir(file_path, root, 6);
  inserir(file_path, root, 5);
  inserir(file_path, root, 4);
  inserir(file_path, root, 3);
  inserir(file_path, root, 2);
  inserir(file_path, root, 1);
  unit_test(file_path, log, errors, test_number, *root, test_6, length);

  delete_file(file_path);
}

void random_insertion_test(char file_path[], FILE* log, int* root, int* errors, int* test_number) {
  int length;
  *root = 0;
  int test_2[] = {1,4};
  length = 2;
	inserir(file_path, root, 1);
	inserir(file_path, root, 1);
	inserir(file_path, root, 4);
  inserir(file_path, root, 4);
  unit_test(file_path, log, errors, test_number, *root, test_2, length);

  int test_3[] = {1,4,7};
  length = 3;
	inserir(file_path, root, 1);
	inserir(file_path, root, 4);
	inserir(file_path, root, 7);
  unit_test(file_path, log, errors, test_number, *root, test_3, length);

  int test_4[] = {1,2,3,4,5,7,9};
  length = 7;
	inserir(file_path, root, 2);
	inserir(file_path, root, 5);
	inserir(file_path, root, 3);
	inserir(file_path, root, 9);
  unit_test(file_path, log, errors, test_number, *root, test_4, length);

  int test_5[] = {1,2,3,4,11,5,6,7,9,13,15,23,17,34};
  length = 14;
	inserir(file_path, root, 11);
	inserir(file_path, root, 15);
	inserir(file_path, root, 13);
	inserir(file_path, root, 17);
	inserir(file_path, root, 23);
	inserir(file_path, root, 6);
	inserir(file_path, root, 34);
	inserir(file_path, root, 34);
	inserir(file_path, root, 11);
  unit_test(file_path, log, errors, test_number, *root, test_5, length);

  int test_6[] = {1,2,3,4,5,6,7,9,11,13,15,17,23,25,28,34,41,38,55};
  length = 19;
	inserir(file_path, root, 41);
	inserir(file_path, root, 25);
	inserir(file_path, root, 55);
	inserir(file_path, root, 28);
	inserir(file_path, root, 38);
  unit_test(file_path, log, errors, test_number, *root, test_6, length);

  int test_7[] = {1,2,3,4,5,6,7,9,11,41,13,15,17,23,25,28,34,38,45,55,67,68,70,78,82};
  length = 25;
	inserir(file_path, root, 45);
	inserir(file_path, root, 67);
	inserir(file_path, root, 78);
	inserir(file_path, root, 82);
	inserir(file_path, root, 70);
	inserir(file_path, root, 78);
	inserir(file_path, root, 68);
  unit_test(file_path, log, errors, test_number, *root, test_7, length);

  int test_8[] = {1,2,3,4,5,6,7,9,11,13,15,17,23,25,28,34,38,41,45,55,67,68,70,78,82,103,123,213,233,365,415,524,819};
  length = 33;
	inserir(file_path, root, 123);
	inserir(file_path, root, 103);
	inserir(file_path, root, 213);
	inserir(file_path, root, 233);
	inserir(file_path, root, 365);
	inserir(file_path, root, 415);
	inserir(file_path, root, 524);
	inserir(file_path, root, 819);
  unit_test(file_path, log, errors, test_number, *root, test_8, length);

  int test_9[] = {1,2,3,4,5,6,7,9,11,13,15,17,23,25,28,34,38,41,45,55,67,68,70,78,82,103, 123,213,254,233,321,365,819,415,524,712,921,1024,1333};
  length = 39;
  inserir(file_path, root, 712);
	inserir(file_path, root, 1024);
	inserir(file_path, root, 1333);
	inserir(file_path, root, 254);
	inserir(file_path, root, 321);
	inserir(file_path, root, 921);
  unit_test(file_path, log, errors, test_number, *root, test_9, length);

  int test_10[] = {1,2,3,4,5,6,7,9,11,23,13,15,17,18,19,20,22,25,27,28,34,38,39,40,41,45,55,67,68,70,78,82,103,819,123,213,254,233,321,365,415,524,712,921,1024,1333,2314,2514,4201,5001};
  length = 50;
  inserir(file_path, root, 2314);
  inserir(file_path, root, 2514);
  inserir(file_path, root, 5001);
  inserir(file_path, root, 40);
  inserir(file_path, root, 39);
  inserir(file_path, root, 27);
  inserir(file_path, root, 22);
  inserir(file_path, root, 20);
  inserir(file_path, root, 18);
  inserir(file_path, root, 19);
  inserir(file_path, root, 4201);
  unit_test(file_path, log, errors, test_number, *root, test_10, length);
  
  delete_file(file_path);
}

int main()
{
    FILE* clean = fopen("minhaarvore.bin","w");
    fclose(clean);
    printf("size of creat_pagina: %llu \n", sizeof(PAGINA));
  int errors = 0;
  int test_number = 1;
	char nomearq[] = "minhaarvore.bin";
	int raiz = 0;

  FILE* log = fopen("log.txt","w+");

  right_insertion_test(nomearq, log, &raiz, &errors, &test_number);
  left_insertion_test(nomearq, log, &raiz, &errors, &test_number);
  random_insertion_test(nomearq, log, &raiz, &errors, &test_number);

  if (errors != 0) {
    printf("\n====================================\nFalhou em %d testes dos %d.\nVerifique o log para maiores informações\n====================================\n", errors, test_number-1);
  } else {
    printf("\n====================================\nPassou em todos os casos de teste :)\n====================================\n");
  }
  
}
