subroutine frequencyCalculation(timesRepeated)
    use global
    use filenameSet
    implicit none
    !parameters for file
    logical::alive
    character(3)::filestatus
    integer::i_output
    !benefit arrary index
    integer::iBenefit, indexBenefit
    integer::numBenefit
    real,allocatable::benefitArray(:)
    !the value b/c begins
    real::benefit_begin 
    real::cost_begin
    !runs (generations) in one repeat
    integer::run
    !number of coorperator
    integer::numCooperator
    !repeated times in one benefit in one realization 
    integer,intent(in)::timesRepeated
    integer::indexRepeated
    !!!averaged quantity in many repeat
    real::sumfrequencyC_run
    real::averageFrequencyC_run
    integer::averageTimes
    !
    real::sumfrequencyC_Repeat
    real::averageFrequencyC_Repeat
    real::sumfrequencyC_network
    real::averageFrequencyC_network
    !
    integer::averageBeginRun
    !
    integer::iDeallocate, fileIndex
    type(temporalNetwork)::networkMain(n)
    integer::activeN
    integer,allocatable::activeNode(:)
    !!!! Store all snapshots
    type(AllTemporalSnapshots), allocatable::AllSnapshots(:)
    !
    real::payoff(n)
    !
    !Output
    character(4)::timeRepeatFile
    character(50)::OutputFilePath
    integer::generationNum
    !The time used to aggregate networks
    integer::deltaT
    integer::totalFileNum
    character(10)::methodDeal
    character(20)::timeWindow
    integer::indexMethod
    !
    character(7)::benefitStr
    !!!! number of repetitions that end in all C, all D, and the mixed case
    integer::numOne, numZero, numMiddle
    !
    numBenefit = 21

    allocate(benefitArray(numBenefit))

    benefit_begin = 1.01
    !set the parameters in one step
    do iBenefit = 1, numBenefit
        benefitArray(iBenefit) = benefit_begin + (real(iBenefit) - 1) * 0.025
    end do

    write(timeWindow,*) timeWindowMain
    ! Aggregated network
    !write(timeWindow,*) "Aggregated"
    
    select case (trim(adjustl(timeWindow)))
        case ("1Hour")
            totalFileNum = 41
        case ("2Hour")
            totalFileNum = 23
        case ("6Hour")
            totalFileNum = 9
        case ("24Hour")
            totalFileNum = 5
        case ("Aggregated")
            totalFileNum = 1
            networkT = 1
            write(*,*) "This program does NOT apply to this case!"
            !stop !!! For aggregated networks, please uncomment this line
        case default
            write(*,*) "-- No such timewindow !! --"
            stop
    end select
    !!
    generationNum = totalFileNum * 2 * 10 ** 5
    
    !!!
    averageBeginRun = generationNum -  2 * 10 ** 3
    
    allocate(AllSnapshots(totalFileNum))
    
    open(unit = 131, file = "filePath"//trim(adjustl(timeWindow))//".txt")
    
    methodLoop:do indexMethod = 1, 1
        !!!!!!
        read(131,"(A150)") filePathPrefix
        !!!!
        write(methodDeal,*) "Original"
        !! ------------------------------------------------------------- !!
        if(indexMethod == 1)then
            write(filenamePrefix,*) "HighSchool2013Network"//trim(adjustl(timeWindow))
        else
            write(filenamePrefix,*) "HighSchool2013Network"//trim(adjustl(timeWindow))//trim(adjustl(methodDeal))
        end if    
        !! ------------------------------------------------------------- !!
        write(OutputFilePath,*) trim(adjustl(methodDeal))//"\"//trim(adjustl(timeWindow))//"\"
        !! ------------------------------------------------------------- !!
        write(*,*) "For "//trim(adjustl(methodDeal))//" "//trim(adjustl(timeWindow))//" network!"
        write(*,*) "Total num of nodes:",n
        write(*,*) "network num:",totalFileNum
        write(*,*) "-------------------"
        write(*,*) filenamePrefix
        write(*,*) "-------------------"
        write(*,*) "Network Generation:", networkT
        write(*,*) "-------------------" 
        write(*,*) "Replicator rule!"
        write(*,*) "-------------------" 
        !! ------------------------------------------------------------- !!
        inquire(file = trim(adjustl(OutputFilePath))//"frequencyC_b", exist = alive)
        if(alive == .false.)then
            filestatus='new'
        else
            filestatus='old'
        end if
        !!Output
        open(unit = 11, file = trim(adjustl(OutputFilePath))//"frequencyC_b",status = filestatus, position = 'append')
        !!
        inquire(file = trim(adjustl(OutputFilePath))//"numDominates", exist = alive)
        if(alive == .false.)then
            filestatus='new'
        else
            filestatus='old'
        end if
        !!Output
        open(unit = 123, file = trim(adjustl(OutputFilePath))//"numDominates",status = filestatus, position = 'append')
        !!
        open(unit = 144, file = trim(adjustl(OutputFilePath))//"frequencyC_b_Repeat")    
        !!
        sumfrequencyC_network = 0.0
        averageFrequencyC_network = 0.0
        !!!
        call RANDOM_SEED()
        benefitLoop:do indexBenefit = 1, numBenefit
                !PGG main parameter
                benefit = benefitArray(indexBenefit)
            
                CC = 1.0
                CD = 0.0 + suckerPayoff
                DC = benefit
                DD = 0.0
            
                write(*,*) "benefit-cost-ratio",benefit / cost
                write(*,*) "S payoff = ", suckerPayoff
                write(benefitStr, "(F7.3)") benefit
            !--------------Repeated in one Benefit in one Realization--------------!
                sumfrequencyC_Repeat = 0.0
                averageFrequencyC_Repeat = 0.0
                
                numZero = 0
                numMiddle = 0
                numOne = 0
                
                timeRepeatedOneRealization:do indexRepeated=1, timesRepeated
                    write(timeRepeatFile,"(I4)")  indexRepeated     
                    !-------Initialize strategies-------!
                    call Inital_strategies()
                    !
                    numCooperator = sum(strategy)
                    !
                    run = 1
                    averageTimes = 0
                    sumfrequencyC_run = 0.0
                    averageFrequencyC_run = 0.0
                    !!!
                    fileIndex = 1
                    call readTemporalActiveNodeNum(fileIndex,activeN)                
                    allocate(activeNode(activeN))
                    call readTemporalNetwork(fileIndex,activeN,networkMain, activeNode)
                    !
                    generationLoop:do while(run <= generationNum)
                        !
                        !!!!! ----------- Change Network After networkT updates ---------------!!!!
                        !!!!!--------------For Aggregated, comment the following code----------!!!!
                        !if(run /= 1 .and. mod(run, networkT) == 1)then
                        !    do iDeallocate = 1, activeN
                        !        deallocate(networkMain(activeNode(iDeallocate))%neighborNode)
                        !        deallocate(networkMain(activeNode(iDeallocate))%neighborTimesInteraction)
                        !    end do
                        !    deallocate(activeNode)
                        !    fileIndex = fileIndex + 1
                        !    !!!
                        !    !!!
                        !    if(fileIndex > totalFileNum)then
                        !        fileIndex = 1
                        !    end if
                        !    !write(*,*) "networkHour",fileIndex
                        !    !pause
                        !    call readTemporalActiveNodeNum(fileIndex,activeN)
                        !    allocate(activeNode(activeN))
                        !    call readTemporalNetwork(fileIndex,activeN,networkMain, activeNode)
                        !    !
                        !end if
                        !!!!!--------------For Aggregated, comment the above code----------!!!!
                        !
                        call PDForHighSchool(activeN,networkMain, activeNode,payoff)
                        !!! Synchronous replicator update rule
                        call SynchronousUpdateForHT(activeN,networkMain, activeNode,payoff)
                        !!------------------------------!!
                        
                        numCooperator = sum(strategy)
                        !!!
                        if(numCooperator == n)then
                            sumfrequencyC_Repeat = sumfrequencyC_Repeat + 1.0
                            numOne = numOne + 1
                            averageFrequencyC_run = 1.0
                            exit generationLoop
                        elseif(numCooperator == 0)then
                            sumfrequencyC_Repeat = sumfrequencyC_Repeat + 0.0
                            numZero = numZero + 1
                            averageFrequencyC_run = 0.0
                            exit generationLoop
                        end if

                        if(run > averageBeginRun)then
                            averageTimes = averageTimes + 1
                            sumfrequencyC_run = sumfrequencyC_run + real(numCooperator)/n
                            if(run == generationNum)then
                                averageFrequencyC_run = sumfrequencyC_run / averageTimes
                                sumfrequencyC_Repeat = sumfrequencyC_Repeat + averageFrequencyC_run
                                numMiddle = numMiddle + 1
                            end if
                        end if
                        run = run + 1
                    end do generationLoop

                    do iDeallocate = 1, activeN
                        deallocate(networkMain(activeNode(iDeallocate))%neighborNode)
                        deallocate(networkMain(activeNode(iDeallocate))%neighborTimesInteraction)
                    end do
                    deallocate(activeNode)

                    write(144,"(1X,F7.3,2X,F7.4)") benefit, averageFrequencyC_run

                end do timeRepeatedOneRealization
                
                averageFrequencyC_Repeat = sumfrequencyC_Repeat / timesRepeated
                write(*,*) "*** FrequencyC per repeat ***"
                write(*,*) averageFrequencyC_Repeat
                
                write(11,"(1X,F7.3,2X,F7.4)") benefit, averageFrequencyC_Repeat
                write(123,"(1X,F7.3,3(2X,I3))") benefit, numZero, numMiddle, numOne
        end do benefitLoop
            
        close(144)
        close(123)
        close(11)
        !
    end do methodLoop

    deallocate(benefitArray)
    
    return 
end subroutine frequencyCalculation