subroutine readTemporalNetwork(fileIndex,activeN, networkMain, activeNode)
    use global
    use filenameSet
    implicit none
    integer,intent(in)::activeN, fileIndex
    type(temporalNetwork),intent(out)::networkMain(n)
    integer,intent(out)::activeNode(activeN)      

    !--------------------------------------------------!
    ! The format of the file read should be            !
    ! line 1 : the total number of active nodes:activeN!
    ! line 2 to end :                                  !
    ! every component includes two lines:              !
    ! node index; node degree                          !
    ! neighbors ...                                    !
    !--------------------------------------------------!
    ! Arrays which save the network informations       !
    ! activeNode(activeN)                              !
    ! activeDegree(activeN)                            !
    !--------------------------------------------------!
    
    integer::nodeIndex
    integer::degreeNow
    integer::nodeNow
    character(5)::networkDay
    character(50)::neighFormat
    
    write(networkDay,"(I5)") fileIndex
    
    degreeMain = 0
    open(unit = 121, file = trim(adjustl(fileNetworkPath)))
    !!!!skip the first line
    read(121,"(I3)") 
    do nodeIndex = 1, activeN
        read(121,"(I3,1X,I3)") activeNode(nodeIndex),degreeNow
        nodeNow = activeNode(nodeIndex)
        degreeMain(nodeNow) = degreeNow
        ! Read Neighbors
        write(neighFormat, *) "(",degreeNow,"(I3,1X))"
        allocate(networkMain(nodeNow)%neighborNode(degreeNow))
        read(121,neighFormat) networkMain(nodeNow)%neighborNode
        ! Read interaction times with neigbhors
        write(neighFormat, *) "(",degreeNow,"(I4,1X))"
        allocate(networkMain(nodeNow)%neighborTimesInteraction(degreeNow))
        read(121,neighFormat) networkMain(nodeNow)%neighborTimesInteraction
        !!! For the union of network snapshots   : set isAggregatedNetworkWeighted = 0 in control.f90
        !!! For the addition of network snapshots: set isAggregatedNetworkWeighted = 1 in control.f90
        networkMain(nodeNow)%neighborTimesInteraction = networkMain(nodeNow)%neighborTimesInteraction ** isAggregatedNetworkWeighted
    end do
    close(121)
    
    return
    end subroutine readTemporalNetwork