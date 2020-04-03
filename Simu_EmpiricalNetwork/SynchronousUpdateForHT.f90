subroutine SynchronousUpdateForHT(activeN,networkMain, activeNode,payoff)
    !!!!! Synchronous replicator update rule !!!!!
    use global
    implicit none
    real,intent(in)::payoff(n)
    integer,intent(in)::activeN
    type(temporalNetwork),intent(in)::networkMain(n)
    integer,intent(in)::activeNode(activeN)
    !------------------------------------------------------------!
    integer::indexNode
    integer::node, degreeFocal, interactionTimesFocal
    integer::interactionTimesNeigh
    integer::maxInteractionTimes
    real::maxPayoff
    real::randSelect
    real::randUpdate, probUpdate
    integer::neighborSelect
    integer,allocatable::neighborNow(:)
    !------------------------------------------------------------------!
    !tempStrategy: temporal strategy is used to save the updated       !
    !strategies and avoid affecting the next updating.                 !
    !------------------------------------------------------------------!
    integer::tempStrategy(n)
    !
    tempStrategy = strategy
    
    update:do indexNode = 1, activeN
        node = activeNode(indexNode)
        degreeFocal = degreeMain(node)
        interactionTimesFocal = sum(networkMain(node)%neighborTimesInteraction)
        
        allocate(neighborNow(degreeFocal))
        neighborNow = networkMain(node)%neighborNode
        
        call RANDOM_NUMBER(randSelect)
        neighborSelect = neighborNow(floor(randSelect * degreeFocal) + 1)
        interactionTimesNeigh = sum(networkMain(neighborSelect)%neighborTimesInteraction)
        !!!
        if (strategy(neighborSelect) /= strategy(node) .and. payoff(neighborSelect) > payoff(node)) then
            !
            maxInteractionTimes =  max(interactionTimesNeigh,interactionTimesFocal) 
            maxPayoff = maxInteractionTimes * (DC - CD)
            !
            probUpdate = (payoff(neighborSelect) - payoff(node)) / maxPayoff          
            call RANDOM_NUMBER(randUpdate)
            if (randUpdate < probUpdate) then
                tempStrategy(node) = strategy(neighborSelect)
            end if
        end if
        deallocate(neighborNow)
    end do update
    !
    strategy = tempStrategy
    !
    return
end subroutine SynchronousUpdateForHT