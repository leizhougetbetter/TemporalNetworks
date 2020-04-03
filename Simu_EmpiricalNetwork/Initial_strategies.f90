subroutine Inital_strategies()
    use global
    implicit none
    !randomly set 50% cooperators and 50% defectors
    !!! if n is odd, n / 2 = (n-1) / 2
    integer::indexC(n / 2)
    integer::indexD(n - n / 2)
    !the nodes(index) that initially are cooperators 
    integer::indexTotal(n)
    integer::indexCount
    !loop parameters
    integer::i, j
    !random number
    real:: randReal
    integer::randInteger
    !!
    integer::numDefector
    real::randCorD
    !!!!!!
    do i = 1, n
        indexTotal(i) = i
    end do
    !
    indexCount = 1
    indexC = -100
    !!!
    ! A
    do while (indexCount <= n / 2)
        call RANDOM_NUMBER(randReal)
        randInteger = floor(randReal * n) + 1
        if(indexTotal(randInteger) /= -1)then
            indexC(indexCount) = randInteger
            indexCount = indexCount + 1
            indexTotal(randInteger) = -1
        end if
    end do
    
    !set the strategy arrary
    strategy = 0 
    do j = 1, n / 2
        !!!
        strategy(indexC(j)) = 1
    end do
    !!! if n is ODD
    indexD = -100
    !!!
    if(mod(n,2) /= 0)then
        numDefector = 0
        do j = 1, n
            if(strategy(j) == 0)then
                numDefector = numDefector + 1
                indexD(numDefector) = j
            end if
        end do
        if(numDefector /= (n - n /2))then
            write(*,*) "Error in Initializing Strategies!"
            stop
        end if
        !!! randomly select an index in indexD
        call RANDOM_NUMBER(randReal)
        randInteger = floor(randReal * numDefector) + 1
        !!! randomly assign strategy C or D to this chosen defector
        call RANDOM_NUMBER(randCorD)
        !!!
        if(randCorD < 0.5)then
            strategy(indexD(randInteger)) = 1
        end if
    end if

    return
end subroutine