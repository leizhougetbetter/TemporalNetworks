subroutine PDForHighSchool(activeN,networkMain, activeNode,payoff)
    use global
    implicit none
    integer,intent(in)::activeN
    type(temporalNetwork),intent(in)::networkMain(n)
    integer,intent(in)::activeNode(activeN)
    real,intent(out)::payoff(n)
    !---------------------------------------------------------!
    integer::indexFocalPlayer,focalPlayer
    integer::strategyFocal
    integer::degreeFocal
    integer::sumTimesInteractionTotal
    integer::sumTimesInteractionWithC, sumTimesInteractionWithD
    !
    payoff = 0.0
    
    do indexFocalPlayer = 1, activeN
        focalPlayer = activeNode(indexFocalPlayer)
        degreeFocal = degreeMain(focalPlayer)
        strategyFocal = strategy(focalPlayer)
        !----------------------calculate payoffs----------------------!
        sumTimesInteractionTotal = sum(networkMain(focalPlayer)%neighborTimesInteraction)
        sumTimesInteractionWithC =  sum(strategy(networkMain(focalPlayer)%neighborNode) * networkMain(focalPlayer)%neighborTimesInteraction)
        sumTimesInteractionWithD = sumTimesInteractionTotal -  sumTimesInteractionWithC

        if(strategyFocal == 1)then
            !!! Focal Player is Cooperator
            payoff(focalPlayer) = sumTimesInteractionWithC * CC + sumTimesInteractionWithD * CD
        else
            !!! Focal Player is Defector
            payoff(focalPlayer) = sumTimesInteractionWithC * DC + sumTimesInteractionWithD * DD
        end if
        !!!! Accumulative Payoffs (defaut setting)
        !!!! Average Payoffs
        !!!!payoff(focalPlayer) = payoff(focalPlayer) / sumTimesInteractionTotal
    end do
    return
end subroutine PDForHighSchool