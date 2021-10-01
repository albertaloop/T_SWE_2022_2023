
//This is our list of messages
//flexcan config script made with help of
//http://eet.etec.wwu.edu/KurtTaylor/project/docs/Bibleography/MCU/ConnSoftDocs/Kinetis%20SDK%20v2.0.0%20API%20Reference%20Manual/group__flexcan__driver.html

enum MovementState
{
    Still = 0,
    Accelerating = 1,
    //note: accelerating can mean deceleration as well
    //we are combining accel and decel to reduce chances of errors
    Cruising = 2
};

enum Msg_Type
{
    None = 0,
    Telemetry = 1,
    Accelerate = 2
    //note: accelerating can mean deceleration as well
    //we are combining accel and decel to reduce chances of errors
};