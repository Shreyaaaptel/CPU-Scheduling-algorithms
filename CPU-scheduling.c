#include<stdio.h>
#include<stdlib.h>

void swap(int *a,int *b)
{
    int temp=*a;
    *a=*b;
    *b=temp;
}

void FCFS(){
    FILE * input_file;

    input_file = fopen("input_FCFS.txt", "r");


    int id[15];
    int burst_time[15];
    int n;

    fseek(input_file, 21, SEEK_CUR);

    fscanf(input_file, "%d",&n);

    fseek(input_file, 34, SEEK_CUR);

    for(int i=0;i<n;i++)
    {
        fscanf(input_file, "%d", &id[i]);
        fseek(input_file, 1, SEEK_CUR);
    }


    fseek(input_file, 13, SEEK_CUR);

    for(int i=0;i<n;i++)
    {
        fscanf(input_file, "%d", &burst_time[i]);
        fseek(input_file, 1, SEEK_CUR);
    }

    int i, wt[n];
    wt[0]=0;

    FILE * output_file;

    output_file = fopen("output_FCFS.txt", "w");

    //for calculating waiting time of each process
    for(i=1; i<n; i++)
    {
        wt[i]= burst_time[i-1]+ wt[i-1];
    }

    fprintf(output_file, "Process ID     Burst Time     Waiting Time     TurnAround Time\n");
    float twt=0.0;
    float tat= 0.0;
    for(i=0; i<n; i++)
    {
        fprintf(output_file, "%d\t\t", id[i]);
        fprintf(output_file, "%d\t\t", burst_time[i]);
        fprintf(output_file, "%d\t\t", wt[i]);

        //calculating and printing turnaround time of each process
        fprintf(output_file, "%d\t\t", burst_time[i]+wt[i]);
        fprintf(output_file, "\n");

        //for calculating total waiting time
        twt += wt[i];

        //for calculating total turnaround time
        tat += (wt[i]+burst_time[i]);
    }
    float att,awt;

    //for calculating average waiting time
    awt = twt/n;

    //for calculating average turnaround time
    att = tat/n;
    fprintf(output_file, "Avgerage waiting time= %f\n",awt);
    fprintf(output_file, "Avgerage turnaround time= %f",att);

    fprintf(output_file, "\n\nGantt Chart:\n");

    for(int i = 0; i<n; i++){
        for(int j=0; j<burst_time[i]; j++){
            fprintf(output_file, "P%d |", id[i]);
        }
    }

    fclose(input_file);
    fclose(output_file);
}

void SJF(){
    FILE * input_file;

    input_file = fopen("input_SJF.txt", "r");

    fseek(input_file, 21, SEEK_CUR);

	int A[100][4];
	int i, j, n, total = 0, index, temp;
	float avg_wt, avg_tat;

    fscanf(input_file, "%d", &n);


	fseek(input_file, 14, SEEK_CUR);

	for (i = 0; i < n; i++) {
		fscanf(input_file, "%d", &A[i][1]);
		fseek(input_file, 1, SEEK_CUR);
		A[i][0] = i + 1;
	}

	fclose(input_file);

	FILE * output_file;

	output_file = fopen("output_SJF.txt", "w");

	// Sorting process according to their Burst Time.
	for (i = 0; i < n; i++) {
		index = i;
		for (j = i + 1; j < n; j++)
			if (A[j][1] < A[index][1])
				index = j;
		temp = A[i][1];
		A[i][1] = A[index][1];
		A[index][1] = temp;

		temp = A[i][0];
		A[i][0] = A[index][0];
		A[index][0] = temp;
	}
	A[0][2] = 0;
	// Calculation of Waiting Times
	for (i = 1; i < n; i++) {
		A[i][2] = 0;
		for (j = 0; j < i; j++)
			A[i][2] += A[j][1];
		total += A[i][2];
	}
	avg_wt = (float)total / n;
	total = 0;
	fputs("P	 BT	 WT	 TAT\n", output_file);
	// Calculation of Turn Around Time and printing the
	// data.
	for (i = 0; i < n; i++) {
		A[i][3] = A[i][1] + A[i][2];
		total += A[i][3];
		fprintf(output_file,"P%d	 %d	 %d	 %d\n", A[i][0],
			A[i][1], A[i][2], A[i][3]);
	}
	avg_tat = (float)total / n;
	fprintf(output_file, "\nAverage Waiting Time = %f", avg_wt);
	fprintf(output_file, "\nAverage Turnaround Time = %f", avg_tat);

    fprintf(output_file, "\n\nGantt Chart:\n");
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < A[i][1]; j++){
            fprintf(output_file, "P%d | ", A[i][0]);
        }
    }
	fclose(input_file);
	fclose(output_file);
}

void RR(){
    FILE *input, *output;
    int pid[100], at[100], bt[100], ct[100], tat[100], wt[100];
    int n, i, j, quantum;

    input = fopen("input_RR.txt", "r");
    output = fopen("output_RR.txt", "w");

    fscanf(input, "%d", &n); //scan number of process
    for (i = 0; i < n; i++) {
        fscanf(input, "%d%d%d", &pid[i], &at[i], &bt[i]); //scan process_id, arrival_time and burst_time
    }
    fscanf(input, "%d", &quantum); //scan time quantum

    // RR Algorithm
    int rt[100];
    for (i = 0; i < n; i++) {
        rt[i] = bt[i];
    }

    int flag = 0;
    int time = 0;
    printf("Gantt Chart\n-----------\n");
    fprintf(output, "Gantt Chart\n-----------\n");
    while (1) {
        flag = 0;
        for (i = 0; i < n; i++) {
            if (rt[i] > 0) {
                flag = 1;
                if (rt[i] > quantum) {
                    time += quantum;
                    rt[i] -= quantum;
                }
                else {
                    time += rt[i];
                    ct[i] = time;
                    tat[i] = ct[i] - at[i];
                    wt[i] = tat[i] - bt[i];
                    rt[i] = 0;

                    // Print the Gantt Chart
                    printf("| P%d ", pid[i]);
                    if (i == n - 1) {
                        printf("|\n");
                    }
                    fprintf(output, "|P%d|", pid[i]);
                }
            }
        }
        if (flag == 0) {
            break;
        }
    }
    fprintf(output, "\n\nProcess\tArrival_Time\tBurst_Time\tCompletion_Time\tTurnaround_Time\tWaiting_Time\n");
    for (i = 0; i < n; i++) {
        fprintf(output, "%d\t\t%d\t\t\t\t%d\t\t\t%d\t\t\t\t%d\t\t\t\t%d\n", pid[i], at[i], bt[i], ct[i], tat[i], wt[i]);
    }

    float total_wt = 0, total_tat = 0;
    for (i = 0; i < n; i++) {
        total_wt += wt[i];
        total_tat += tat[i];
    }
    fprintf(output, "Average waiting time = %f\n", total_wt / n);
    fprintf(output, "Average turnaround time = %f\n", total_tat / n);

    fclose(input);
    fclose(output);
}

void SRTF(){
    FILE *input, *output;
    int pid[100], at[100], bt[100], rt[100], ct[100], tat[100], wt[100];
    int n, time, smallest, i, j, total_wt = 0, total_tat = 0;

    input = fopen("input_SRTF.txt", "r");
    output = fopen("output_SRTF.txt", "w");

    fscanf(input, "%d", &n); //scan number of process
    for (i = 0; i < n; i++) {
        fscanf(input, "%d%d%d", &pid[i], &at[i], &bt[i]); //scan process_id, arrival_time and burst_time
        rt[i] = bt[i];
    }

    // SRTF Algorithm
    int complete = 0, min_bt = 1000, index = 0;
    int start[100] = {0};
    printf("Gantt Chart\n-----------\n");
    fprintf(output, "Gantt Chart\n-----------\n");
    for (time = 0; complete != n; time++) {
        min_bt = 1000;
        for (i = 0; i < n; i++) {
            if (at[i] <= time && rt[i] < min_bt && rt[i] > 0) {
                min_bt = rt[i];
                index = i;
            }
        }
        rt[index]--;
        if (rt[index] == 0) {
            complete++;
            ct[index] = time + 1;
            tat[index] = ct[index] - at[index];
            wt[index] = tat[index] - bt[index];
            total_wt += wt[index];
            total_tat += tat[index];
        }

        if (index != j) {
            start[index] = time;
            printf("| P%d ", index);
            fprintf(output, "| P%d ", index);
            j = index;
        } else if (rt[index] == 0) {
            printf("|     ");
            fprintf(output, "|     ");
        }
    }
    printf("|\n");

    fprintf(output, "\n\nProcess\tArrival_Time\tBurst_Time\tCompletion_Time\tTurnaround_Time\tWaiting_Time\n");
    for (i = 0; i < n; i++) {
        fprintf(output, "%d\t\t%d\t\t\t\t%d\t\t\t%d\t\t\t\t%d\t\t\t\t%d\n", pid[i], at[i], bt[i], ct[i], tat[i], wt[i]);
    }
    fprintf(output, "Average waiting time = %f\n", (float)total_wt / n);
    fprintf(output, "Average turnaround time = %f\n", (float)total_tat / n);

    fclose(input);
    fclose(output);
}

void Priority_Scheduling(){
    FILE * input_file;

    input_file = fopen("input_priority.txt", "r");

    fseek(input_file, 21, SEEK_CUR);

    int n;

    fscanf(input_file, "%d", &n);

    fseek(input_file, 14, SEEK_CUR);

    // b is array for burst time, p for priority and index for process id
    int b[n],p[n],index[n];
    for(int i=0;i<n;i++)
    {
        fscanf(input_file, "%d",&b[i]);
        fseek(input_file, 1, SEEK_CUR);
        index[i]=i+1;
    }

    fseek(input_file, 12, SEEK_CUR);

    for(int i=0;i<n;i++)
    {
        fscanf(input_file, "%d", &p[i]);
    }
    for(int i=0;i<n;i++)
    {
        int a=p[i],m=i;

        //Finding out highest priority element and placing it at its desired position
        for(int j=i;j<n;j++)
        {
            if(p[j] > a)
            {
                a=p[j];
                m=j;
            }
        }

        //Swapping processes
        swap(&p[i], &p[m]);
        swap(&b[i], &b[m]);
        swap(&index[i],&index[m]);
    }

    // T stores the starting time of process
    int t=0;

    //Printing scheduled process
    FILE * output_file;

    output_file = fopen("output_priority.txt", "w");

    fprintf(output_file, "Order of process Execution is\n");
    for(int i=0;i<n;i++)
    {
        fprintf(output_file, "P%d is executed from %d to %d\n",index[i],t,t+b[i]);
        t+=b[i];
    }
    fprintf(output_file, "\n");
    fprintf(output_file, "Process Id     Burst Time   Wait Time    TurnAround Time\n");
    int wait_time=0;
    for(int i=0;i<n;i++)
    {
        fprintf(output_file, "P%d             %d           %d            %d\n",index[i],b[i],wait_time,wait_time + b[i]);
        wait_time += b[i];
    }
    fprintf(output_file, "\nGantt Chart:\n");
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<b[i]; j++)
        {
            fprintf(output_file, "P%d | ", index[i]);
        }
    }
    fclose(input_file);
    fclose(output_file);
}

void LRTF(){
    struct Process
{
    int Process_ID;
    int Burst_Time;
    int WaitingTime;
    int TurnAroundTime;
};
    FILE * input_file;

    input_file = fopen("input_LRTF.txt", "r");

    fseek(input_file, 21, SEEK_CUR);

    struct Process data[20];
    int n;
    fscanf(input_file, "%d", &n);

    fseek(input_file, 14, SEEK_CUR);

    for (int i = 0; i < n; i++)
    {
        fscanf(input_file, "%d", &data[i].Burst_Time);
        fseek(input_file, 1, SEEK_CUR);
    }

    fseek(input_file, 13, SEEK_CUR);

    for (int i = 0; i < n; i++)
    {
        fscanf(input_file, "%d", &data[i].Process_ID);
        fseek(input_file, 1, SEEK_CUR);
    }

    struct Process temp;

    FILE * output_file;

    output_file = fopen("output_LRTF.txt", "w");


    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < (n - 1-i); j++)
        {
            if (data[j].Burst_Time < data[j + 1].Burst_Time)
            {
                temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
            else if(data[j].Burst_Time == data[j + 1].Burst_Time)
            {
            	if(data[j].Process_ID > data[j + 1].Process_ID)
            	{
            	temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
                }
			}
        }
    }

    data[0].WaitingTime=0;

    for(int j=1; j<n; j++)
    {
        data[j].WaitingTime = data[j-1].WaitingTime + data[j-1].Burst_Time;
    }


    for(int j=0; j<n; j++)
    {
        data[j].TurnAroundTime=data[j].WaitingTime+data[j].Burst_Time;
    }

    int i,AvgWaitingTime=0,AvgTurnAroundTime=0;
	int TotalWatingTime=0,TotalTurnAroundTime=0;
    fprintf(output_file, "Student id\tBurst Time\tWaitingTime\tTurnAroundTime");
    for (i = 0; i < n; i++)
    {
        fprintf(output_file, "\n%d\t\t\t%d\t\t\t%d\t\t\t%d\t\t", data[i].Process_ID, data[i].Burst_Time,data[i].WaitingTime,data[i].TurnAroundTime);
		TotalWatingTime= TotalWatingTime+data[i].WaitingTime;
		TotalTurnAroundTime= TotalTurnAroundTime+data[i].TurnAroundTime;
	}

	fprintf(output_file, "\n\nAverage Waiting Time is: = %d",TotalWatingTime);
	fprintf(output_file, "\nAverage Turn around Time is: = %d\n\n",TotalTurnAroundTime);

    fprintf(output_file, "Gantt Chart:\n");
    for(int i = 0; i<n; i++)
    {
        for(int j = 0; j<data[i].Burst_Time; j++)
        {
            fprintf(output_file, "P%d | ", data[i].Process_ID);
        }
    }
}

int main(){
    int key;
    printf("Which algorithm would you like to use?\n\n");
    printf("1)FCFS\n2)SJF\n3)RR\n4)SRTF\n5)Priority Scheduling\n6)LRTF\n\nEnter: ");
    scanf("%d", &key);
    switch (key)
    {
    case 1:
        FCFS();
        break;
    case 2:
        SJF();
        break;
    case 3:
        RR();
        break;
    case 4:
        SRTF();
        break;
    case 5:
        Priority_Scheduling();
        break;
    case 6:
        LRTF();
        break;
    default:
        break;
    }
    return 0;
}
