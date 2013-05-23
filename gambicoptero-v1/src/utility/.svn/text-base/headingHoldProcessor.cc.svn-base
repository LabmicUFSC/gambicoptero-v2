#include<utility/headingHoldProcessor.h>
#include<mach/mc13224v/itg3200.h>
#include<utility/math.h>

__USING_SYS

void HeadingHoldProcessor::processHeading(ITG3200 *gyro, FlightControlQuad *fcq){



    if (headingHoldConfig == AeroQuad::ON) {
        db<HeadingHoldProcessor>(TRC) << "--- processHeading ---" << endl;

            heading = Math::degrees(gyro->gyroHeading);
            db<HeadingHoldProcessor> (TRC) << "heading degrees: " << heading << endl;

            // Always center relative heading around absolute heading chosen during yaw command
            // This assumes that an incorrect yaw can't be forced on the AeroQuad >180 or <-180 degrees
            // This is done so that AeroQuad does not accidentally hit transition between 0 and 360 or -180 and 180
            // AKA - THERE IS A BUG HERE - if relative heading is greater than 180 degrees, the PID will swing from negative to positive
            // Doubt that will happen as it would have to be uncommanded.
            relativeHeading = heading - setHeading;
            if (heading <= (setHeading - 180)) {
                relativeHeading += 360;
            }
            if (heading >= (setHeading + 180)) {
                relativeHeading -= 360;
            }

            // Apply heading hold only when throttle high enough to start flight
                db<HeadingHoldProcessor>(TRC) << "ReceiverCommand THROTTLE: " << _receiver->_receiverCommandPC[Receiver::THROTTLE] << endl;
            if (_receiver->_receiverCommandPC[Receiver::THROTTLE] > Receiver::MINCHECK ) {

                if ((_receiver->_receiverCommandPC[AeroQuad::ZAXIS] > (Receiver::MIDCOMMAND + 25)) ||
                        (_receiver->_receiverCommand[AeroQuad::ZAXIS] < (Receiver::MIDCOMMAND - 25))) {

                    // If commanding yaw, turn off heading hold and store latest heading
                    setHeading = heading;
                    headingHold = 0;
                    _pid->PID[PIDclass::HEADING_HOLD_PID_IDX].integratedError = 0;
                    headingHoldState = AeroQuad::OFF;
                    headingTime = AeroQuad::_currentTime;
                }
                else {
                    if (relativeHeading < 0.25 && relativeHeading > -0.25) {
                        headingHold = 0;
                        _pid->PID[PIDclass::HEADING_HOLD_PID_IDX].integratedError = 0;
                    }
                    else if (headingHoldState == AeroQuad::OFF) { // quick fix to soften heading hold on new heading
                        if ((AeroQuad::_currentTime - headingTime) > 500000) {
                            headingHoldState = AeroQuad::ON;
                            headingTime = AeroQuad::_currentTime;
                            setHeading = heading;
                            headingHold = 0;
                        }
                    }
                    else {
                        // No new yaw input, calculate current heading vs. desired heading heading hold
                        // Relative heading is always centered around zero
                        headingHold = _pid->updatePID(0, relativeHeading, PIDclass::HEADING_HOLD_PID_IDX);//);&PID[HEADING_HOLD_PID_IDX]);
                        headingTime = AeroQuad::_currentTime; // quick fix to soften heading hold, wait 100ms before applying heading hold
                    }
                }
            }
            else {
                // minimum throttle not reached, use off settings
                setHeading = heading;
                headingHold = 0;
                _pid->PID[PIDclass::HEADING_HOLD_PID_IDX].integratedError = 0;
            }
        }
        // NEW SI Version
        const float commandedYaw = AQMath::constrain(_receiver->getReceiverSIData(AeroQuad::ZAXIS) +
                                             Math::radians(headingHold), -Math::PI, Math::PI);
        fcq->motorAxisCommandYaw = _pid->updatePID(commandedYaw, gyro->gyroRate[AeroQuad::ZAXIS]*0.8,
                                                   PIDclass::ZAXIS_PID_IDX);//&PID[ZAXIS_PID_IDX]);
        db<HeadingHoldProcessor>(TRC) << "Process Heading: CommandYaw :" << fcq->motorAxisCommandYaw;
    }



