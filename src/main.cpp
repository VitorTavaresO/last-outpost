#include <last-outpost/game_control.h>

namespace Game
{
	int main(int argc, char **argv)
	{
		GameControl gameControl;

		if (!gameControl.initialize())
		{
			return 1;
		}

		gameControl.run();
		return 0;
	}
}

int main(int argc, char **argv)
{
	return Game::main(argc, argv);
}