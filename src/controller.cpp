#include "controller.h"

#include <iostream>

using namespace std;

Controller::Controller(QObject* parent): QObject(parent),
	_next_step(Step::Start)
{
	_player = new PlaySound("нажмите кнопку", this);
	connect(_player, &PlaySound::sound_ended, this, &Controller::instruction_ends);
	step();
}


void Controller::press_center()
{
	_next_step = Step::Right;
	step();
}

void Controller::press_top()
{
	_next_step = Step::Bottom;
	step();
}

void Controller::press_bottom()
{
	_next_step = Step::Left;
	step();
}

void Controller::press_left()
{
	_next_step = Step::Start;
	step();
}

void Controller::press_right()
{
	_next_step = Step::Top;
	step();
}

void Controller::instruction_ends()
{
	_curr_step = _next_step;
	emit step_changed();
}


void Controller::step()
{
	if(_next_step == _saved_step){
		return;
	}
	switch (_next_step)
	{
	case Step::Start:
		step_start();
		break;
	case Step::Right:
		step_right();
		break;
	case Step::Left:
		step_left();
		break;
	case Step::Top:
		step_top();
		break;
	case Step::Bottom:
		step_bottom();
		break;
	default:
		cerr << "wrong step number!" << endl;
		break;
	}
	_saved_step = _next_step;
}

void Controller::step_start()
{
	(*_player << "для выполнения тестовой процедуры вам необходимо последовательно, в соответствии с командами нажать на кнопки"
		<< "справа" << "сверху" << "снизу" << "слева").play();
	_player->play("для начала нажмите центральную кнопку");
}

void Controller::step_right()
{
	*_player << "нажмите кнопку" << "справа";
	_player->play();
}

void Controller::step_left()
{
	*_player << "нажмите кнопку" << "слева";
	_player->play();
}

void Controller::step_top()
{
	*_player << "нажмите кнопку" << "сверху";
	_player->play();
}

void Controller::step_bottom()
{
	*_player << "нажмите кнопку" << "снизу";
	_player->play();
}
