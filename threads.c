#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

double diffTimeSec(struct timespec t0, struct timespec t1) {
    return ((double) t1.tv_sec - t0.tv_sec) + ((double) (t1.tv_nsec - t0.tv_nsec) * 1e-9);
}

typedef struct {
    int *f_num;
    int *l_num;
    int *jump_len;
} thread_lenght_struct;

int define_num_type(int num) {
    int div_sum=0;
    for(int i=1; i<num; i++) {
        if((num%i)==0){
            div_sum += i;
        }
    }
    if(div_sum==num) {
        return 1; 
    } else if(div_sum>num) {
        return 2; 
    } else if(div_sum<num) {
        return 3; 
    }
    return 0;
}
double sequencial(int worksize) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    int p_count=0, d_count=0, a_count=0, type;
    for(int num=1; num<=worksize; num++) {
        type = define_num_type(num);
        switch (type) {
            case 1:
                p_count++;
                break;
            case 2:
                a_count++;
                break; 
            case 3:
                d_count++;
                break;
        }
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    printf("----------------------------------------------\n");
    printf("* Sequential\t[P]\t[A]\t[D]\t[WTot]\t\n");
    printf("\t\t%d\t%d\t%d\t%d\t\n",p_count,a_count,d_count,worksize);
    return diffTimeSec(start, end);
}
void * chunk_thread(void *args) {
    thread_lenght_struct *actual_args = args;
    int type;
    int f_num = *(actual_args->f_num);
    int l_num = *(actual_args->l_num);
    int *v_types = malloc (sizeof (int) * 4); 
    v_types[0] = 0;
    v_types[1] = 0;
    v_types[2] = 0;
    v_types[3] = l_num-f_num;
    for(int num=f_num;num<l_num;num++) {
        type = define_num_type(num);
        switch (type) {
            case 1:
                v_types[0]++;
                break;
            case 2:
                v_types[1]++;
                break; 
            case 3:
                v_types[2]++;
                break;
        }  
    }

    free(actual_args);
    return v_types;
}
double chunk(int worksize, int num_thr) {
    struct timespec start, end;
    pthread_t *thr_vect;
    thr_vect = (pthread_t*)malloc(num_thr * sizeof(pthread_t));
    int *vect, v_slice, var_size, i;
    vect = (int*)malloc(num_thr * sizeof(int));
    if(worksize%num_thr==0) {
        v_slice = worksize/num_thr; 
        for(i=0;i<num_thr;i++) {
            vect[i] = (i+1)*v_slice+1;
        }
    } else {
        var_size = worksize-(worksize%num_thr);
        v_slice = var_size/num_thr; 
        for(i=0;i<num_thr;i++) {
            vect[i] = (i+1)*v_slice+1;
        }
        vect[num_thr-1] += (worksize-var_size);
    }
    printf("----------------------------------------------\n");
    printf("* Chunk \t[P]\t[A]\t[D]\t[WTh]\n");
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    for(i=0;i<num_thr;i++) {
        thread_lenght_struct *args = malloc(sizeof *args);
        if(i==0) {
            int f_num = 1;
            args->f_num = &f_num;
            args->l_num = &vect[i];
            if(pthread_create(&thr_vect[i], NULL, chunk_thread, args)){
                free(args);
            }
        } else {
            args->f_num = &vect[i-1];
            args->l_num = &vect[i];
            if(pthread_create(&thr_vect[i], NULL, chunk_thread, args)){
                free(args);
            }
        }
    }
    int p_count=0, a_count=0, d_count=0;
    for(i=0;i<num_thr;i++) {          
        void *status = NULL;
        pthread_join(thr_vect[i], &status);
        p_count += ((int*)status)[0];
        a_count += ((int*)status)[1];
        d_count += ((int*)status)[2];
        printf("- Thread [%d]: \t%d\t%d\t%d\t%d\t\n",i+1,((int*)status)[0],((int*)status)[1],((int*)status)[2],((int*)status)[3]);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    printf("  [TOTAL] \t%d\t%d\t%d\t%d\n",p_count,a_count,d_count,worksize);

    free(thr_vect);
    free(vect);
    return diffTimeSec(start, end);
}
void * esparsa_thread(void *args) {
    thread_lenght_struct *actual_args = args;
    int type;
    int f_num = *(actual_args->f_num);
    int l_num = *(actual_args->l_num);
    int jump_len = *(actual_args->jump_len);
    int *v_types = malloc (sizeof (int) * 3); 
    v_types[0] = 0;
    v_types[1] = 0;
    v_types[2] = 0;
    for (int num=f_num;num<=l_num;num+=jump_len) {
        type = define_num_type(num);
        switch (type) {
            case 1:
                v_types[0]++;
                break;
            case 2:
                v_types[1]++;
                break; 
            case 3:
                v_types[2]++;
                break;
        }  
        v_types[3]++;
    }

    free(actual_args);
    return v_types;
}

double esparsa(int worksize, int num_thr) {
    struct timespec start, end;
    pthread_t *thr_vect;
    thr_vect = (pthread_t*)malloc(num_thr * sizeof(pthread_t));
    int *start_vect;
    start_vect = (int*)malloc(num_thr * sizeof(int));
    printf("----------------------------------------------\n");
    printf("* Sparse \t[P]\t[A]\t[D]\t[WTh]\n");
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    for(int i=0;i<num_thr;i++) {
        start_vect[i]=i+1;
        thread_lenght_struct *args = malloc(sizeof *args);
        args->f_num = &start_vect[i];
        args->l_num = &worksize;
        args->jump_len = &num_thr;
        if(pthread_create(&thr_vect[i], NULL, esparsa_thread, args)){
            free(args);
        }
    }
    int p_count=0, a_count=0, d_count=0;
    for(int i=0;i<num_thr;i++) {          
        void *status = NULL;
        pthread_join(thr_vect[i], &status);
        p_count += ((int*)status)[0];
        a_count += ((int*)status)[1];
        d_count += ((int*)status)[2];
        printf("- Thread [%d]: \t%d\t%d\t%d\t%d\t\n",i+1,((int*)status)[0],((int*)status)[1],((int*)status)[2],((int*)status)[3]);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    printf("  [TOTAL] \t%d\t%d\t%d\t%d\n",p_count,a_count,d_count,worksize);
    printf("----------------------------------------------\n");

    free(thr_vect);
    free(start_vect);
    return diffTimeSec(start, end);
}

int main(int argc, char **argv) {
    int worksize, num_thr;
    double seq_time, chunk_time, esp_time;
    if (argc == 3 && strtol(argv[1], NULL, 10) != 0) {
        num_thr = abs(strtol(argv[1], NULL, 10));
        worksize = abs(strtol(argv[2], NULL, 10));
    } else if(argc > 1 && strtol(argv[1], NULL, 10)==0) {
        printf("The number of threads must be greater than 0.\n");
        exit(0);
    }
    else {
        printf("The number of threads and the worksize must be passed as arguments.\n");
        printf("Ex.: ./<file-name> 4 100\n");
        exit(0);
    }
    seq_time = sequencial(worksize);
    chunk_time = chunk(worksize, num_thr);
    esp_time = esparsa(worksize, num_thr);

    printf("Sequential time:  %lfs\n",seq_time);
    printf("Using threads time: %lfs (chunk distribution)\n",chunk_time);
    printf("Using threads time: %lfs (sparse distribution)\n",esp_time);
}