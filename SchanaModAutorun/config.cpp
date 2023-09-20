class CfgPatches
{
	class SchanaModAutorun
	{
		requiredAddons[] = {};
		units[] = {};
		weapons[] = {};
	};
};

class CfgMods
{
	class SchanaModAutorun
	{
		type = "mod";
		inputs = "SchanaModAutorun/Data/Inputs.xml";
		dependencies[] =
		{
			"World",
			"Mission"
		};
		class defs
		{
			class worldScriptModule
			{
				files[] =
				{
					"SchanaModAutorun/Scripts/4_world"
				};
			};
			class missionScriptModule
			{
				files[] =
				{
					"SchanaModAutorun/Scripts/5_mission"
				};
			};
		};
	};
};
