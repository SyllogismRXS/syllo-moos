Multiple Waypoint Behaviors Demo
==================================

1) Launch the mission

$ ./launch.sh

2) Click the DEPLOY button

3) After the vehicle has completed at least one waypt, click the SECOND
button. The vehicle will now begin traversing a different set of waypts.

4) Click the CONTINUE button. The vehicle will go back to the original set of
waypts, remembering the points it has already completed. You can switch between
the two waypt sets by toggling between the SECOND and CONTINUE buttons.

5) Click the RETURN button to return to the start position. Click DEPLOY to
start again.

How does it work?
------------------

This example mission was modified from the original moos-ivp s2_bravo mission
if you want to compare the differences. Basically, an additional BHV_Waypoint
behavior was added and is enabled when the MODE is set to "SECOND_SURVEYING."
The MODE is controlled by the state machine at the top of the bravo.bhv
file. The buttons in pMarineViewer set the DEPLOY, SECOND, and RETURN
variables. Look at the pMarineViewer block in bravo.moos to see how the
button_[one,two,three,four] actions set the relevant variables.

If you want to control switching between the waypoint sets autonomously, set
SECOND to the appropriate value (true or false) when you send the updated
waypoint list. Just make sure the "updates" parameter in the BHV_Waypoint
blocks are unique.
