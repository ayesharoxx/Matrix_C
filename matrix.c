#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h> /* for ENOSYS */

int matrix_allocate(matrix_t *m, int rows, int columns) {
    m->rows = rows;
    m->columns = columns;    
    m->content = malloc(rows * sizeof(int *));
    if(m->content == NULL || rows <= 0 || columns <= 0){
        return -1;
    }
    for(int i = 0; i < rows; i++){
        m->content[i] = malloc(columns * sizeof(int));
        if(m->content[i] == NULL || rows <= 0 || columns <= 0){
            return -1;
        }
    }
    return 0;
    /* implement the function here ... */

    /*return -ENOSYS;*/ /* ENOSYS is a  standard error code that means 'function
                       not implemented'. We use it to force the failure of tests
                       while the functions are not written. Make sure to remove
                       it when you start to work on a function */
}

void matrix_free(matrix_t *m) {  
   for(int i=0; i<m->rows; i++) {
        free(m->content[i]);
   
   }
   free(m->content);
    /* implement the function here ... */
}

void matrix_init_zeros(matrix_t *m) {
   for(int i=0; i<m->rows; i++) {
        for(int j=0; j<m->columns; j++){
            m->content[i][j] = 0;
        }
    }
    /* implement the function here ... */
}


int matrix_init_identity(matrix_t *m)
{   
    
    if(m->rows != m->columns){
        return 1;
        
    }
    for(int i=0; i<m->rows; i++) {
        for(int j=0; j<m->columns; j++){
            if(i==j){
                
                
                m->content[i][j] = 1;
                }
            else{
               
                m->content[i][j] = 0;
                }
        }
    }
    return 0;
}
void matrix_init_rand(matrix_t *m) {
    for(int i=0; i<m->rows; i++) {
        for(int j=0; j<m->columns; j++){
            m->content[i][j] = rand() % 201 - 100;
        }
    }
  
    /* implement the function here ... */
}

int matrix_equal(matrix_t *m1, matrix_t *m2) {
    /* implement the function here ... */
    if(m1->rows != m2->rows || m1->columns != m2->columns){
        return 0;
        }
    for(int i=0; i<m1->rows; i++) {
        for(int j=0; j<m1->columns; j++){
            if(m1->content[i][j] != m2->content[i][j]){
                return 0;
                }
        }
    }
   
    return 1;


}

int matrix_sum(matrix_t *m1, matrix_t *m2, matrix_t *result) {
    if(m1 == NULL || m2 == NULL || m1->content == NULL || m2->content == NULL){
        return 1;
        }
    if(m1->rows != m2->rows || m1->columns != m2->columns){
        return 1;
        }
    if(matrix_allocate(result, m1->rows, m1->columns)){
        return 1;
        }

    for(int i=0; i<m1->rows; i++) {
        for(int j=0; j<m1->columns; j++){
            result->content[i][j] = m1->content[i][j] + m2->content[i][j];
        }
    }
    return 0;
  
}

int matrix_scalar_product(matrix_t *m, int scalar, matrix_t *result) {
    /* implement the function here ... */
    if(m == NULL || m->content == NULL){
        return 1;
        }
    if(matrix_allocate(result, m->rows, m->columns)){
        return 1;
        }
    for(int i=0; i<m->rows; i++) {
        for(int j=0; j<m->columns; j++){
            //scalar product
            result->content[i][j] = m->content[i][j] * scalar;
        }
    }
    return 0;
}

int matrix_transposition(matrix_t *m, matrix_t *result) {
    /* implement the function here ... */
    if(m == NULL || m->content == NULL){
        return 1;
        }
    if(matrix_allocate(result, m->columns, m->rows)){
        return 1;
        }
 
    for(int i=0; i<m->rows; i++) {
        for(int j=0; j<m->columns; j++){
            result->content[j][i] = m->content[i][j];
        }
    }
    return 0;
}

int matrix_product(matrix_t *m1, matrix_t *m2, matrix_t *result) {
    /* implement the function here ... */
    if(m1 == NULL || m2 == NULL || m1->content == NULL || m2->content == NULL){
        return 1;
        }
   
    if(m1->columns != m2->rows){
        return 1;
        }
    if(matrix_allocate(result, m1->rows, m2->columns)){
        return 1;
       }

    for(int i=0; i<m1->rows; i++) {
        for(int j=0; j<m2->columns; j++){
            result->content[i][j] = 0;
            for(int k=0; k<m1->columns; k++){
                result->content[i][j] += m1->content[i][k] * m2->content[k][j];
            }
        }
    }
    return 0;
}

int matrix_dump_file(matrix_t *m, char *output_file) {
     if(m == NULL || m->content == NULL){
       return 1;
     } 
     FILE *fptr;
     char wbuffer[(((sizeof(int)*m->columns)+m->columns)*m->rows)-1];
     strcpy(wbuffer,"");
     char val[15];
     
     fptr = fopen(output_file,"w");
     if(!fptr){
       return 1;
     }
     for(int i = 0; i<m->rows; i++){
       for(int j = 0; j<m->columns; j++){
        
          if (j == m->columns-1){
          	snprintf(val,15,"%i\n",m->content[i][j]);
          }
          else{
                snprintf(val,15,"%i ",m->content[i][j]);
          }
          strncat(wbuffer,val,15);
       }
     
     }
     if(fwrite(wbuffer,1,strlen(wbuffer),fptr)!=strlen(wbuffer)){
         return 1;
     }
     fclose(fptr);
     return 0;

  }



int matrix_allocate_and_init_file(matrix_t *m, char *input_file) {
    
    FILE *fp;
    int row = 0, col = 1;
    char ch;
    char prevch='x';
    fp = fopen(input_file,"r+");
    if(fp==NULL) return 1;
    int next_col=1;
     
    while( (ch = fgetc(fp)) != EOF ){
        if (!isdigit(ch) && ch!=' ' && ch!='\n' && ch!='-')
            return 1;
        if(  row == 0 && ch==' ' && prevch!=' ')
            col++;
        else if( row!=0 && ch==' ' && prevch!=' ')
        {
            next_col++;
           
        }
        else if( ch == '\n' && prevch==' ')
            continue;
        else if(ch == '\n' && prevch=='\n')
            continue;
        else if(ch == '\n' && prevch!=' ')
        {
            if(row!=0 && next_col!=col)
                return 1; 
            next_col=1;
            row++;
        }
        prevch=ch;
    }
    fseek(fp, 0, SEEK_SET);
   
    if(matrix_allocate(m,row,col)!=-1)
    {
        for(int i=0;i<m->rows;i++)
            for(int j=0;j<m->columns;j++)
                fscanf(fp,"%d", &m->content[i][j]);
        return 0;
    }
    else
        return 1;
    fclose(fp);
   
    
}

   
