modded class MissionGameplay extends MissionBase
{
    ref array<string> m_SchanaAutorunInterruptInputs;

    override void OnInit()
    {
        super.OnInit();
        m_SchanaAutorunInterruptInputs = new array<string>;
        m_SchanaAutorunInterruptInputs.Insert("UASchanaAutorunToggle");
        m_SchanaAutorunInterruptInputs.Insert("UAMoveForward");
        m_SchanaAutorunInterruptInputs.Insert("UAMoveBack");
        m_SchanaAutorunInterruptInputs.Insert("UAMoveLeft");
        m_SchanaAutorunInterruptInputs.Insert("UAMoveRight");
        m_SchanaAutorunInterruptInputs.Insert("UAFire");
        m_SchanaAutorunInterruptInputs.Insert("UATempRaiseWeapon");
    }

    protected bool SchanaCheckInput(string inputName)
    {
        if(GetUApi())
		{
            UAInput inp = GetUApi().GetInputByName(inputName);

            if(inp && inp.LocalPress())
            {
                return true;
            }
        }
        return false;
    }

    protected bool SchanaAutorunShouldInterrupt(PlayerBase player)
    {
        if(!player.SchanaIsAutorunning())
        {
            return false;
        }

        // Check player status
        if(player.IsInVehicle() || player.IsUnconscious() || player.IsRestrained() || !player.IsAlive())
        {
            return true;
        }

        // Check inputs
        for(int i=0; i<m_SchanaAutorunInterruptInputs.Count(); i++)
        {
            string inputName = m_SchanaAutorunInterruptInputs[i];
            
            if(SchanaCheckInput(inputName))
            {
                return true;
            }
        }
        return false;
    }

    protected bool SchanaAutorunShouldStart(PlayerBase player)
    {
        if(player.SchanaIsAutorunning())
        {
            return false;
        }

        string autorunStartInput = m_SchanaAutorunInterruptInputs[0];
        if(SchanaCheckInput(autorunStartInput) && !player.SchanaIsAutorunning() && !IsControlDisabled())
        {
            return true;
        }
        return false;
    }

    protected int SchanaAutorunGetUpdatedSpeed(PlayerBase player)
    {
        if(player.GetStaminaHandler().GetStamina() <= 0)
        {
            return DayZPlayerConstants.MOVEMENTIDX_RUN;
        }
        if(SchanaCheckInput("UAWalkRunToggle"))
        {
            return DayZPlayerConstants.MOVEMENTIDX_WALK;
        }
        else if(SchanaCheckInput("UAWalkRunTemp"))
        {
            return DayZPlayerConstants.MOVEMENTIDX_RUN;
        }
        else if(SchanaCheckInput("UATurbo"))
        {
            return DayZPlayerConstants.MOVEMENTIDX_SPRINT;
        }
        else
        {
            // Used to indicate that there is to be no update to the speed
            return DayZPlayerConstants.MOVEMENTIDX_IDLE;
        }
    }

    protected void SchanaAutorunStart()
    {
        SchanaAutorunSync(true, DayZPlayerConstants.MOVEMENTIDX_SPRINT, 1);
    }

    protected void SchanaAutorunStop()
    {
        SchanaAutorunSync(false, DayZPlayerConstants.MOVEMENTIDX_IDLE, 0);
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());

        if (player)
        {
            if(SchanaAutorunShouldInterrupt(player))
            {
                SchanaAutorunStop();
            }
            else if(SchanaAutorunShouldStart(player))
            {
                SchanaAutorunStart();
            }
            else if(player.SchanaIsAutorunning())
            {
                int speed = SchanaAutorunGetUpdatedSpeed(player);
                if(speed != DayZPlayerConstants.MOVEMENTIDX_IDLE)
                {
                    SchanaAutorunSync(true, speed, 1);
                }
            }
        }
    }

    protected void SchanaAutorunSync(bool isRunning, int movementIdx, int angle)
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());

        player.SetSchanaAutorunning(isRunning);
        player.GetInputController().OverrideMovementSpeed(isRunning, movementIdx);
        player.GetInputController().OverrideMovementAngle(isRunning, angle);

        auto params = new Param3<bool, int, int>(isRunning, movementIdx, angle);
        GetGame().RPCSingleParam(player, -44201, params, true);
    }
}