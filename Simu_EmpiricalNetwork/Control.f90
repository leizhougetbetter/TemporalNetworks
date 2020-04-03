!-----------------------------------------------------------------------!
! This program deals with the games on empirical temporal network.      !
! The data set here is HighSchool2013.                                  !
!-----------------------------------------------------------------------!
! Each \Delta t snapshot lasts for T (parameter g in the maintext)      !
! generations.                                                          !
!-----------------------------------------------------------------------!
module global
    implicit none
    integer,parameter:: n = 327
    real,parameter :: cost = 1.0
    integer,save::strategy(n)
    integer,save::degreeMain(n)
    character(10),save::timeWindowMain
    real,save::benefit
    !!!
    integer,save::networkT
    integer,save::isAggregatedNetworkWeighted = 0
    real, save::suckerPayoff = 0.0
    !!!
    type temporalNetwork
        integer,allocatable::neighborNode(:)
        integer,allocatable::neighborTimesInteraction(:)
    end type
    
    type AllTemporalSnapshots
        type(temporalNetwork)::networkMain(n)
    end type
    !
    !
    real,save::CC,CD,DC,DD
    !
    end module
    
module filenameSet
    character(50),save::filenamePrefix
    character(150),save::fileNetworkPath
    character(150),save::filePathPrefix
    end module

program main
    use global
    implicit none
    integer::timesRepeated
    integer::averageDegree
    namelist /keyParameters/ timeWindowMain, networkT, isAggregatedNetworkWeighted, suckerPayoff
    character(19)::inputFileName
    logical::alive
    !!!!! number of repetitions
    timesRepeated = 500
    !
    inputFileName = 'temporalNetPars.inp'
    inquire(file = inputFileName, exist = alive)
    if(.not. alive)then
        write(*,*) "ERROR: File "//inputFileName//" is NOT found!"
        stop
    end if
    open(unit = 188, file = inputFileName,status = 'old')
    read(188, nml = keyParameters) 
    close(188)    
    !!!
    write(*,*)
    write(*,*) "-----------------------------"
    write(*,"('NetworkT = ',I5)")  networkT
    write(*,*) "-----------------------------"
    write(*,"('Aggregated Snapshots = ',I1)") isAggregatedNetworkWeighted
    write(*,*) "-----------------------------"
    !!!
    call frequencyCalculation(timesRepeated)
    
    
    stop
end
    