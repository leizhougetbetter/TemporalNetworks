DESCRIPTION
-----------

This is a set of Fortran programs designed for the production of paper titled Evolution of Cooperation on Temporal Networks. 

FILES
-----

Control.f90                        define the global parameters, read the input parameters

frequencyCalculations.f90          main program to calculate the average frequency of cooperators
    
Initial_strategies.f90             set the initial conditions, i.e., 50%  of population are cooperators

PDForHighSchool.f90                subroutine to calculate the payoffs of each individual at each generation

SynchronousUpdateForHT.f90         subroutine to update the strategies of each individual after the games at each generation. The update rule is the Replicator rule

readActiveNodeNum.f90              read the number of (active) nodes in each snapshot

readTemporalNetwork.f90            read and store the information of all snapshots 

temporalNetPars.inp                file for input parameters

filePath1Hour.txt                  the file path of the folder for snapshots under the time window of 1Hour

Original                           folder for the output of the program, its subfolders store the outputs for different time windows (e.g., 1Hour, 2Hour)

RUNNING & OUTPUTS
------------

To start running this program

    *) put all the files listed above in the same working directory
    *) set appropriate parameter values in the temporalNetPars.inp file
    *) set an appropriate file path in the filePathXHour.txt (here, X must be consistent with the time-window parameter used in the temporalNetPars.inp file)
    *) compile the Fortran program and start running

Outputs of this program:
    *) the outputs of this program must be stored in the folder \Original\XHour\ (X=1, 2, 6, or 24) or \Original\aggregated\ under the same working directory
    *) Output frequencyC_b stores the mean frequency of cooperators averaged over all the repetitions for each $b$ ($b$ is the only parameter in the payoff matrix)
    *) Output frequencyC_b_Repeat stores the frequency of cooperators for each repetition 
    *) Output numDominate stores the number of times that the program ends in all-C state, coexistence, and all-D state over all repetitions

QUESTIONS
---------

For any question about this Fortran program, please contact

Dr. Lei Zhou, Email: weifengzhl2012@gmail.com


