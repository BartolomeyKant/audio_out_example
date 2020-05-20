#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QQueue>
#include <QByteArray>
#include <QThread>

class Player: public QObject {
	Q_OBJECT

public: /* constructor */
	Player(const QString& sample_file, QObject* parent=nullptr);

signals:
	void noise_ended();

	void start_play(QPrivateSignal);

public: /* methods */
	void noise(const QString& file);
	void silence();

	bool is_playing() const;

private: /* */
	void queue_file(const QString& file);

private slots:
	void play();

private: /* members */
	QAudioFormat _format;
	QAudioOutput* _aout;
	QIODevice* _audio_dev = nullptr;

	QQueue<QByteArray> _playing_data;
	QByteArray _current_data;

	uint _header_offset;
	int _actual_frame_count;

	const uint PLAY_FRAMES = 1024;
};

#endif /* PLAYER_H */
