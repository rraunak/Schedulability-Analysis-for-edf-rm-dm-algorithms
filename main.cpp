#include <bits/stdc++.h>

using namespace std;

/*Function for utilization bound test*/
bool ubTest(vector<vector<double>>& tasks){
    cout<<"Applying Utilization Bound Test"<<"\n";
    double utilization=0;

    /*Calculate total utilization */
    for(double i=0; i<tasks.size(); i++){
        utilization += tasks[i][0]/min(tasks[i][1],tasks[i][2]);
    }

    /*Print successful if less than 1*/
    if(utilization<=1){
        cout<<"Utilization Bound Test Successful, Utilization<=1"<<"\n";
        return true;
    }
    cout<<"Utilization Bound Test Failed, Utilization>1"<<"\n";
    return false;

}

/*Function for loading factor test*/
bool loadingFactorTest(vector<vector<double>>& tasks){

    cout<<"Applying Loading Factor Test"<<"\n";
    double execTime=0;
    /*sum of execution times*/
    for(double i=0; i<tasks.size(); i++){
        execTime += tasks[i][0];
    }
    int prev=0;
    int curr=execTime;
    int temp;
    /*Calculate synchronous busy period*/
    while (prev != curr){

        prev = curr;
        temp = 0;
        for(double i=0; i<tasks.size(); i++) {
            temp += ceil(curr/tasks[i][2])*tasks[i][0];
        }
        curr = temp;
    }
    cout<<"Synchronous Busy Period: "<<curr<<"\n";
    vector<pair<double, double>> deadline;

    /*push the deadline along with index in an array*/
    for(double i=0; i<tasks.size(); i++){

        deadline.push_back({tasks[i][1], i});

    }

    /*sort the array according to the deadline*/
    sort(deadline.begin(), deadline.end());
    int num=0;
    queue<pair<double, double>> Q;

    /*fill the queue with deadlines*/
    for(double i=0; i<deadline.size(); i++){
        Q.push(deadline[i]);
    }
    vector<double> t;
    vector<double> h;
    double sumh=0;
    double sumt;

    /*Calculate h and t and fill in the array*/
    while(num<=prev){

        /*extract the smallest deadline from the queue*/
        pair<double, double> temp = Q.front();
        Q.pop();
        /*calculate h and t only if the deadline is less than synchronous busy period*/
        if(temp.first<=prev) {
            sumh += tasks[temp.second][0];

            h.push_back(sumh);
            sumt = temp.first;
            /*push the popped t in the array*/
            t.push_back(sumt);

            /*clear the deadline array*/
            deadline.clear();

            /*fill the deadline array from queue*/
            while (!Q.empty()){
                deadline.push_back(Q.front());
                Q.pop();
            }

            double add = temp.first + tasks[temp.second][2];

            /*add the next deadline to the array*/
            deadline.push_back({add, temp.second});

            /*again sort the array*/
            sort(deadline.begin(), deadline.end());

            /*push in the queue the elements of array in the sorted order*/
            for(double i=0; i<deadline.size(); i++){
                Q.push(deadline[i]);
            }
        }

        /*set num as element popped from the queue*/
        num = temp.first;
    }

    vector<double> tx;
    vector<double> hx;

    /*eliminate the duplicate deadlines value in the array*/
    for(double i=1; i<h.size(); i++){

        if(t[i-1] == t[i]){
            continue;
        }

        tx.push_back(t[i-1]);
        hx.push_back(h[i-1]);
    }
    tx.push_back(t[t.size()-1]);
    hx.push_back(h[h.size()-1]);
    double u;

    /*calculate the utilization at each deadline instant*/
    for(double i=0; i<hx.size(); i++){
        cout<<"t"<<i<<":"<<tx[i]<<" ";
        cout<<"h"<<i<<":"<<hx[i]<<" ";
        u = hx[i]/tx[i];
        cout<<"u"<<i<<":"<<u<<"\n";

        /*print failed if the utilization is greater than 1*/
        if(u>1){
            cout<<"Loading Factor Test failed as u>1 for task "<<i<<"\n";
            cout<<"First Missing Deadline: "<<tx[i]<<"\n";
            return false;
        }
    }
    return true;

}

/*Function to check if the deadline is equal to period*/
bool deadlinePeriodCheck(vector<vector<double>>& tasks){

    cout<<"Checking if Deadline == Period"<<"\n";
    for(double i=0; i<tasks.size(); i++){
        if(tasks[i][1] < tasks[i][2]){
            cout<<"Found Deadline < Period"<<"\n";
            return false;
        }
    }
    cout<<"Found Deadline == Period"<<"\n";
    return true;
}

/*Function for edf testing*/
bool edfCheck(vector<vector<double>>& tasks){

    /*check whether deadline is equal to period*/
    if(deadlinePeriodCheck(tasks)){

        /*do utilization bound test*/
        if(ubTest(tasks)){
            return true;
        } else{
            return false;
        }

    } else {

        /*do utilization bound test*/
        if(ubTest(tasks)) {
            return true;
        }
        else {

            /*do loading factor test if utilization bound test fails*/
            if(loadingFactorTest(tasks)){
                return true;
            } else{
                return false;
            }

        }
    }


}

/*Function to calculate utilization for fixed priority scheduling algorithm*/
bool ubTestRmDm(vector<vector<double>>& tasks){

    double utilization=0;
    /*Calculate utilization*/
    for(double i=0; i<tasks.size(); i++){
        utilization += tasks[i][0]/min(tasks[i][1],tasks[i][2]);
    }

    double s = tasks.size();

    /*Apply formula based on number of tasks*/
    double check = s*(pow(2, 1/s) - 1);

    /*print successful if less than check value*/
    if(utilization<=check){
        cout<<"Utilization Test Successful, Utilization<="<<check<<"\n";
        return true;
    }
    cout<<"Utilization Test Unsuccessful, Utilization>"<<check<<"\n";
    return false;


}

/*Function for response time test*/
bool responseTimeTest(vector<vector<double>>& tasks){

    cout<<"Applying Response Time Test"<<"\n";
    double execTime=0;
    int index;
    int maxExec=0;

    /*Calculate total execution time and execution time of task to be tested and it's index*/
    for(double i=0; i<tasks.size(); i++){
        execTime += tasks[i][0];

        if(i == tasks.size()-1){
            maxExec = tasks[i][0];
            index = i;
        }
    }
    int prev=0;
    int curr=execTime;
    int temp;

    /*Calculate worst case response time for the particular task*/
    while (prev != curr){

        prev = curr;

        temp = maxExec;
        for(double i=0; i<tasks.size(); i++) {
            if(i != index) {
                temp += ceil(curr / tasks[i][2]) * tasks[i][0];
            }
        }
        curr = temp;
        /*break if the response time is greater than the period of task tested*/
        if(curr>tasks[index][2]){
        	break;
        }
    }
    cout<<"Worst Case Response Time:"<<curr<<"\n";

    /*print successful if the worst case response time is less than or equal to the period of task tested*/
    if(curr<=tasks[index][2]){
        cout<<"Response Time Test Successful as "<<curr<<"<="<<tasks[index][2]<<"\n";
        return true;
    }
    cout<<"Response Time Test Unsuccessful as "<<curr<<">"<<tasks[index][2]<<"\n";
    return false;

}

/*Function for rm/dm testing*/
bool rm_dmCheck(vector<vector<double>>& tasks){

    /*check for utilization test for rm/dm*/
	if(ubTestRmDm(tasks)){
	    return true;
	} else{

        /*apply response time test if utilization test fails*/
	    if(responseTimeTest(tasks)){
	        return true;
	    } else{
	        return false;
	    }

	}
        

}

/*start of main*/
int main(int argc, char **argv) {

    /*take input arguments in string */
    string input = "";
    string output = "";
    if(argc == 3) {
        input = argv[1];
        output = argv[2];
    } else{
        cout<< "Wrong Input Format: Retry with, ./assignment4 input output\n";
        return 1;
    }

    /*convert input arguments from string to char* */
    const char* inputFile = input.c_str();
    const char* outputFile = output.c_str();

    double x;

    int totalTasksSet=0;
    int tasksNum=0;
    ifstream file;

    /*open the A4_test.txt file*/
    file.open(inputFile);
    if (!file) {
        cerr << "Unable to open file A4_test.txt\n";
        exit(1);   // call system to stop
    }

    /*read the total number of tasksets*/
    file>>x;
    totalTasksSet=x;
    vector<vector<double>> tasks;
    vector<double> t;

    /*write output to report.txt file*/
    std::ofstream out(outputFile);
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());
    std::ignore = coutbuf;

    /*Iterate through each taskset at a time*/
    for(int i=0; i<totalTasksSet; i++){

        /*read the number of tasks in each taskset*/
        file>>x;
        tasksNum=x;

        /*make a 2d array for each tasks containing execution time, deadline and period for a particular taskset*/
        for(int j=0; j<tasksNum; j++){
            for(int k=0; k<3; k++){
                file>>x;

                t.push_back(x);
            }
            tasks.push_back(t);
            t.clear();
        }
        cout<<"\n";
        cout<<"\n";

        /*Print the particular taskset*/
        cout<<"**********************Taskset "<<i<<"***************************\n";
        for(double k=0; k<tasks.size(); k++){
            cout<<"Task "<<k<<":"<<tasks[k][0]<<", "<<tasks[k][1]<<", "<<tasks[k][2]<<"\n";
        }
	cout<<"\n";

        /*Apply the edf algorithm to the particular taskset*/
        cout<<"***********Applying EDF Algorithm for taskset "<<i<<"***********\n";
        if(edfCheck(tasks)){
            cout<<"Taskset "<<i<<" Schedulable through EDF Algorithm\n";
        } else{
            cout<<"Taskset "<<i<<" Not Schedulable through EDF Algorithm\n";
        }
        cout<<"\n";

        /*Apply the RM/DM algorithm to the particular taskset*/
        cout<<"*********Applying RM/DM Algorithm for taskset "<<i<<"***********\n";

        /*Check for utilization bound test*/
        if(ubTest(tasks)){

            /*Choose optimal algorithm on the basis of period and deadline*/
            if(deadlinePeriodCheck(tasks)){
                cout<<"RM is the optimal algorithm"<<"\n";
            } else{
                cout<<"DM is the optimal algorithm"<<"\n";
            }
            vector<vector<double>> tasksnum;

            /*Do RM/DM testing starting from the first task*/
            for(double j=0; j<tasks.size(); j++){

                tasksnum.push_back(tasks[j]);
                cout<<"Applying Utilization Test for"<<" Task "<<j<<" \n";
                if(rm_dmCheck(tasksnum)){
                    cout<<"Task "<<j<<" Schedulable through RM/DM Algorithm\n";
                } else{
                    cout<<"Task "<<j<<" Not Schedulable through RM/DM Algorithm\n";
                }
            }
	    }
        else{
          cout<<"Taskset "<<i<<" Not Schedulable through RM/DM Algorithm\n";
            }
        cout<<"\n";
        cout<<"\n";

        /*clear the tasks vector for next set of taskset*/
        tasks.clear();
    }
    /*clear the output buffer*/
    out.flush();
    return 0;
}
