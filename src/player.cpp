#include "src/player.h"

#include <iostream>

#include <QFile>

#include <exception>
#include <string>

using namespace std;

struct wave_header {
	char chunk_id[4];
	uint32_t chunk_size;
	char format[4];
	char subchunk_id[4];
	uint32_t subchunk_size;
	uint16_t audio_format;
	uint16_t num_channels;
	uint32_t sample_rate;
	uint32_t byte_rate;
	uint16_t block_align;
	uint16_t bits_persample;
	char subchank2_id[4];
	uint32_t subchunk2_size;
};

struct next_chunk
{
	char chunk_id[4];
	uint32_t chunk_size;
};


QString supported_format()
{
	return "suppported format:\n"
	       "\ttype WAVE\n"
	       "\tformat PCM\n"
	       "\tchannels 1\n";
}

Player::Player(const QString& sample_file, QObject* parent): QObject(parent)
{
	QFile file(sample_file);
	if (!file.open(QFile::ReadOnly)) {
		throw std::runtime_error("file " + sample_file.toStdString() +
		                         " can not be open");
	}

	wave_header _hdr;
	if (file.read(reinterpret_cast<char*>(&_hdr), sizeof(wave_header)) !=
	    sizeof(wave_header)) {
		throw std::runtime_error("file " + sample_file.toStdString() +
		                         " has unsupported format, expected .wav file");
	}

	if (QString::fromLatin1(_hdr.chunk_id, 4) != "RIFF") {
		throw std::runtime_error("not RIFF " + supported_format().toStdString());
	}
	if (QString::fromLatin1(_hdr.format, 4) != "WAVE") {
		throw std::runtime_error("not WAVE " + supported_format().toStdString());
	}
	if (_hdr.audio_format != 1) {
		throw std::runtime_error("not PCM " + supported_format().toStdString());
	}
	if (_hdr.num_channels != 1) {
		throw std::runtime_error("not 1  channel (not mono) " +
		                         supported_format().toStdString());
	}

	_header_offset = sizeof(wave_header);

	if (QString::fromLatin1(_hdr.subchank2_id, 4) != "data") {

		cout << "additional header " << endl;
		next_chunk chunk;
		file.skip(_hdr.subchunk2_size);
		_header_offset += _hdr.subchunk2_size + sizeof(next_chunk);
		file.read(reinterpret_cast<char*>(&chunk), sizeof(next_chunk));

		// clang-format off
		while (QString::fromStdString(std::string(chunk.chunk_id, 4)) != "data"
				&& !file.atEnd()) { // clang-format on
			file.skip(chunk.chunk_size);
			file.read(reinterpret_cast<char*>(&chunk), sizeof(next_chunk));
		}
		if(file.atEnd()){
			throw std::runtime_error("file " + sample_file.toStdString() +
		                         " has unsupported format, expected .wav file");
		}
	}
	file.close();

	_format.setSampleRate(_hdr.sample_rate);
	_format.setSampleSize(_hdr.bits_persample);
	_format.setChannelCount(1);
	_format.setCodec("audio/pcm");
	_format.setByteOrder(QAudioFormat::Endian::LittleEndian);
	_format.setSampleType(QAudioFormat::SampleType::SignedInt);

	_actual_frame_count = PLAY_FRAMES;

	_aout = new QAudioOutput(_format);
	_aout->setBufferSize(_format.bytesForFrames(_actual_frame_count));
	_aout->setNotifyInterval(_format.durationForFrames(_actual_frame_count)/1000);

	connect(this, &Player::start_play, this, &Player::play);

	_audio_dev = _aout->start();
}


void Player::noise(const QString& file)
{
	queue_file(file);
	// emit start_play({});
}

void Player::silence()
{
	_playing_data.clear();
}

bool Player::is_playing() const
{
	return !_playing_data.isEmpty();
}

void Player::queue_file(const QString& file)
{
	QFile _file(file);
	if(!_file.open(QFile::ReadOnly))
	{
		throw std::runtime_error("file " + file.toStdString() +
		                         " can not be open");
	}
	_file.skip(_header_offset);
	while (!_file.atEnd()) {
		_playing_data.enqueue(_file.read(_format.bytesForFrames(_actual_frame_count)));
	}
	_file.close();
}

void Player::play()
{
	while(!_playing_data.isEmpty()){
		_current_data = _playing_data.dequeue();

		while(!_current_data.isEmpty()){
			if(_aout->bytesFree() == 0){
				QThread::usleep(_format.durationForBytes(_current_data.size()));
				continue;
			}
			int size = _aout->bytesFree() < _current_data.size() ?
								_aout->bytesFree() :
								_current_data.size();
			int res = _audio_dev->write(_current_data.left(size));
			_current_data = _current_data.right(_current_data.size()-size);
		}
	}

	emit noise_ended();
}
