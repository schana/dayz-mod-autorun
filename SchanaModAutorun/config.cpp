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
					"SchanaModAutorun/World"
				};
			};
			class missionScriptModule
			{
				files[] =
				{
					"SchanaModAutorun/Mission"
				};
			};
		};
	};
};
