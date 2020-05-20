#include <QFile>
#include <exception>
#include <QThread>
#include <QStandardPaths>
#include <QDir>

#include <iostream>

#include "src/play_sound.h"

#include "config.h"

using namespace std;

QHash<QString, QString> PlaySound::sound_map;
QString PlaySound::sound_file_prefix;

PlaySound::PlaySound(const QString& sample_phrase,QObject* parent) : QObject(parent)
{
	if (!sound_map.size() != 0) {
		try {
			find_sound_files();
			init_sound_map(sound_file_prefix + "sound_map");
		}
		catch (std::exception& e) {
			cerr << "ERROR: sounds cannot be loaded "
			       << e.what() << " try to reinstall terminal application" << endl;
			throw e;
		}
	}

	if (!sound_map.contains(sample_phrase)) {
		throw std::runtime_error("there is no sound for phrase " +
		                         sample_phrase.toStdString());
	}

	_player = new Player(sound_map.value(sample_phrase));
	_player->moveToThread(_sound_thrd = new QThread());
	connect(_player, &Player::noise_ended, this, &PlaySound::sound_ended);
	_sound_thrd->start();

}

void PlaySound::find_sound_files()
{
	if (!sound_file_prefix.isEmpty()) {
		return;
	}

	if(!QDir(RESOURCE_DIR).exists()){
		throw std::runtime_error("resource dirrectory [" RESOURCE_DIR "] does not exists");
	}
	sound_file_prefix = RESOURCE_DIR"/";
}

void PlaySound::init_sound_map(const QString& file)
{
	// init sound map from resource file
	QFile s_map(file);
	if (!s_map.open(QFile::ReadOnly | QFile::Text)) {
		throw std::runtime_error("can't open sound map file " + file.toStdString());
	}

	while (!s_map.atEnd()) {
		QString _data = s_map.readLine();
		if (_data == "") {
			continue;
		}
		QStringList list = _data.trimmed().split(":");
		if (list.size() < 2) {
			continue;
		}
		sound_map[list[0]] = sound_file_prefix + list[1];
	}
	s_map.close();
}

PlaySound& PlaySound::operator<<(const QString& phrase)
{
	_phrases.enqueue(phrase);
	return *this;
}
void PlaySound::play(const QString& phrase)
{
	_phrases.enqueue(phrase);
	play();
}

void PlaySound::play()
{
	if (_phrases.isEmpty()) {
		return;
	}
	while(!_phrases.isEmpty()){
		QString phrase = _phrases.dequeue();
		if (!sound_map.contains(phrase)) {
			throw std::runtime_error("there is no sound for phrase " +
									phrase.toStdString());
		}
		cout << "Play phrase " << phrase.toStdString();
		_player->noise(sound_map.value(phrase));
	}
	_player->start_play({});
}

bool PlaySound::is_playing() const
{
	return _player->is_playing();
}

void PlaySound::stop()
{
	_phrases.clear();
	_player->silence();
}
