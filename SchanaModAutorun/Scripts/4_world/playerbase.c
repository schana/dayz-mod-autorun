modded class PlayerBase extends ManBase
{
    protected bool m_SchanaAutorunning;

    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        if (rpc_type == -44201)
        {
            Param3<bool, int, int> autorunParams;
            if(!ctx.Read(autorunParams)) return;
            bool isRunning = autorunParams.param1;
            int movementIdx = autorunParams.param2;
            int angle = autorunParams.param3;
            this.GetInputController().OverrideMovementSpeed(isRunning, movementIdx);
            this.GetInputController().OverrideMovementAngle(isRunning, angle);
        }

        super.OnRPC(sender, rpc_type, ctx);
    }

    void SetSchanaAutorunning(bool isRunning)
    {
        m_SchanaAutorunning = isRunning;
    }

    bool SchanaIsAutorunning()
    {
        return m_SchanaAutorunning;
    }
}