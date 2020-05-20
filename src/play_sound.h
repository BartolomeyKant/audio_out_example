#ifndef PLAY_SOUND_HH
#define PLAY_SOUND_HH

#include <QObject>
#include <QHash>
#include <QQueue>
#include <QString>

#include "src/player.h"

class PlaySound: public QObject {
	Q_OBJECT

public: /* constructor */
	PlaySound(const QString& sample_phrase, QObject* parent = nullptr);

signals:
	void sound_ended();

public: /* methods */
	PlaySound& operator<<(const QString& phrase);

	void play(const QString& phrase);
	void play();
	void stop();

	bool is_playing() const;

private: /* methods */
	void init_sound_map(const QString& file);
	void find_sound_files();

private: /* members */
	QQueue<QString> _phrases;
	static QHash<QString, QString> sound_map;
	static QString sound_file_prefix;

	QThread* _sound_thrd;
	Player* _player;
};

#endif /* PLAY_SOUND_HH */
