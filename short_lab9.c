#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NO_OF_QUEUE       20	/* #of Students */
#define NO_OF_SLOTS       5

int     Q[NO_OF_SLOTS];
int     front = -1;           
int     rear  = -1;           

pthread_mutex_t mutex;

/**/

sem_t sem;

void *Supervisor(void *arg){
	int student = *((long*)arg);

	/*Sem*/

	sem_wait(&sem);

	/*mutex*/

	pthread_mutex_lock(&mutex);

	rear = rear + 1;              
	if (rear >= NO_OF_SLOTS) rear = 0;		
	Q[rear] = student;

	printf("%4d. Student was called to the Presentation\n", student+1);

	/* Mutex*/

	pthread_mutex_unlock(&mutex);
          
} 

void *Student(void *arg){
	int student = 0;
    	int studentsRemoved = 0;

	printf("Presentation  is started\n");


	while (studentsRemoved < NO_OF_QUEUE){


	/*mutex lock*/

		pthread_mutex_lock(&mutex);
		
		front = front + 1;           
		if (front >= NO_OF_SLOTS)	front = 0;		
		student = Q[front];
		printf(" %4d. Student is done. It was removed from the Queue \n", student+1);
		studentsRemoved++;

		/*semaphore*/

		sem_post(&sem);

		/*mutex unlock*/

		pthread_mutex_unlock(&mutex);
		
		usleep((int)(drand48()*500000));
	}
} 

int main(int argc,char *argv[]){
	int i;
	pthread_t supervisorId;
	pthread_t QUEUEId;
	int x[NO_OF_QUEUE];

	/*sem*/
	sem_init(&sem, 0, NO_OF_SLOTS);

	/*mutex*/

	pthread_mutex_init(&mutex, NULL);
	
	srand48(time(NULL));

	/* supervisor thread */
	pthread_create(&QUEUEId, NULL, Student, NULL);
	
	for(i=0; i < NO_OF_QUEUE;i++){
		x[i] = i;
		pthread_create(&supervisorId, NULL, Supervisor, (void *) &(x[i]));
	} 
	
	pthread_join(QUEUEId, NULL);

	return 0;
} 

