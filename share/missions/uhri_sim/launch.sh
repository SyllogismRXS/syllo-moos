#!/bin/bash 

#-------------------------------------------------------
#  Part 1: Check for and handle command-line arguments
#-------------------------------------------------------
TIME_WARP=1
JUST_MAKE="no"
VARIATION="0"
for ARGI; do
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
	printf "%s [SWITCHES] [time_warp]   \n" $0
	printf "  --just_make, -j    \n" 
	printf "  --help, -h         \n" 
	exit 0;
    elif [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$TIME_WARP" = 1 ]; then 
        TIME_WARP=$ARGI
    elif [ "${ARGI}" = "v1" ] ; then
	VARIATION="1"
    elif [ "${ARGI}" = "v2" ] ; then
	VARIATION="2"
    elif [ "${ARGI}" = "v3" ] ; then
	VARIATION="3"
    elif [ "${ARGI}" = "--just_build" -o "${ARGI}" = "-j" ] ; then
	JUST_MAKE="yes"
    else 
	printf "Bad Argument: %s \n" $ARGI
	exit 0
    fi
done

#-------------------------------------------------------
#  Part 2: Create the .moos and .bhv files. 
#-------------------------------------------------------
VNAME1="diver"           # The first vehicle Community
VNAME2="videoray"           # The second vehicle Community
START_POS1="10,0"         
START_POS2="0,0"        
LOITER_POS1="x=19,y=0"
LOITER_POS2="x=25,y=0"
SHORE_LISTEN="9300"

nsplug meta_vehicle.moos targ_diver.moos -f WARP=$TIME_WARP \
    VNAME=$VNAME1          SHARE_LISTEN="9301"              \
    VPORT="9001"           SHORE_LISTEN=$SHORE_LISTEN       \
    START_POS=$START_POS1  VARIATION=$VARIATION             

nsplug meta_vehicle.moos targ_videoray.moos -f WARP=$TIME_WARP \
    VNAME=$VNAME2          SHARE_LISTEN="9302"              \
    VPORT="9002"           SHORE_LISTEN=$SHORE_LISTEN       \
    START_POS=$START_POS2  VARIATION=$VARIATION             

nsplug meta_shoreside.moos targ_shoreside.moos -f WARP=$TIME_WARP \
    SNAME="shoreside"  SHARE_LISTEN=$SHORE_LISTEN                 \
    SPORT="9000"       VARIATION=$VARIATION         

nsplug meta_vehicle.bhv targ_diver.bhv -f VNAME=$VNAME1     \
    START_POS=$START_POS1 LOITER_POS=$LOITER_POS1           \
    TRAIL_ENABLED=0

nsplug meta_vehicle.bhv targ_videoray.bhv -f VNAME=$VNAME2     \
    START_POS=$START_POS1 LOITER_POS=$LOITER_POS2              \
    TRAIL_ENABLED=1

if [ ${JUST_MAKE} = "yes" ] ; then
    exit 0
fi

#-------------------------------------------------------
#  Part 3: Launch the processes
#-------------------------------------------------------
printf "Launching $SNAME MOOS Community (WARP=%s) \n"  $TIME_WARP
pAntler targ_shoreside.moos >& /dev/null &
sleep 0.25
printf "Launching $VNAME1 MOOS Community (WARP=%s) \n" $TIME_WARP
pAntler targ_diver.moos >& /dev/null &
sleep 0.25
printf "Launching $VNAME2 MOOS Community (WARP=%s) \n" $TIME_WARP
pAntler targ_videoray.moos >& /dev/null &
printf "Done \n"

#-------------------------------------------------------
# Start ROS Processes in new window
#-------------------------------------------------------
gnome-terminal --title="ROS" -e "bash -c \"roslaunch videoray morse-moos-ros.launch; exec bash\""

uMAC targ_shoreside.moos

wmctrl -c ROS

printf "Killing all processes ... \n"
kill %1 %2 %3
printf "Done killing processes.   \n"


