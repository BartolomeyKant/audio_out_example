#ifndef CONTROLLER_HH
#define CONTROLLER_HH

#include <QObject>
#include <QString>

#include "src/play_sound.h"

class Controller: public QObject {
	Q_OBJECT
	Q_PROPERTY(Step step READ curr_step NOTIFY step_changed)

public:
	enum class Step {
		None,
		Start,
		Right,
		Left,
		Top,
		Bottom,
		Last,
	};
	Q_ENUMS(Step)

public: /* contructor */
	Controller(QObject* parent = nullptr);

signals:
	void step_changed();
public slots:
	void instruction_ends();

public: /* methods */
	Q_INVOKABLE void press_center();
	Q_INVOKABLE void press_top();
	Q_INVOKABLE void press_bottom();
	Q_INVOKABLE void press_left();
	Q_INVOKABLE void press_right();

	Step curr_step() {
		return _curr_step;
	}

private: /* methods */
	void step();

	void step_start();
	void step_right();
	void step_left();
	void step_top();
	void step_bottom();

private: /* members */
	Step _next_step = Step::None;
	Step _saved_step = Step::None;
	Step _curr_step = Step::None;

	PlaySound* _player;
};
#endif /* CONTROLLER_HH */
