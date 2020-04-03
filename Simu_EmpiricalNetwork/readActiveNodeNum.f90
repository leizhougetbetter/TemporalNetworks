subroutine readTemporalActiveNodeNum(fileIndex,activeNum)
    use filenameSet
    implicit none
    integer,intent(in)::fileIndex
    integer,intent(out)::activeNum         

    character(5)::networkIndex
    write(networkIndex,"(I5)") fileIndex
    
    fileNetworkPath = trim(adjustl(filePathPrefix))//trim(adjustl(filenamePrefix))//trim(adjustl(networkIndex))
    
    !!! The first line of the file is the number of active nodes
    open(unit = 121, file = trim(adjustl(fileNetworkPath)))
    read(121,"(I3)") activeNum
    close(121)
    
    return
    end subroutine readTemporalActiveNodeNum