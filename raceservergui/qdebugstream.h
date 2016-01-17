#ifndef QDEBUGSTREAM_H
#define QDEBUGSTREAM_H

#include <iostream>
#include <streambuf>
#include <string>
#include <mutex>

#include <qtextedit.h>
#include <qstring.h>
#include <qscrollbar.h>

class QDebugStream : public std::basic_streambuf<char>
{
public:
	QDebugStream(std::ostream &stream, QTextEdit* text_edit, QColor c = Qt::black) : m_stream(stream) {
		log_window = text_edit;
		m_old_buf = stream.rdbuf();
		stream.rdbuf(this);
		msgColor = c;
	}
	~QDebugStream() {
		// output anything that is left
		loglock.lock();
		if (!m_string.empty()){
		//	QScrollBar * sb = log_window->verticalScrollBar();
			//bool atMax = (sb->value() == sb->maximum()) ? true : false;
			log_window->setTextColor(msgColor);
			log_window->append(QString::fromStdString(m_string.c_str()));
		//	/*if (atMax)*/ sb->setValue(sb->maximum());
		}
		m_stream.rdbuf(m_old_buf);
		loglock.unlock();
	}

protected:
	virtual int_type overflow(int_type v) {
		loglock.lock();
		if (v == '\n') {
		//	QScrollBar * sb = log_window->verticalScrollBar();
			//bool atMax = (sb->value() == sb->maximum()) ? true : false;
			log_window->setTextColor(msgColor);
			log_window->append(QString::fromStdString(m_string.c_str()));
		//	/*if (atMax)*/ sb->setValue(sb->maximum());
			m_string.erase(m_string.begin(), m_string.end());
		}
		else
			m_string += v;

		loglock.unlock();
		return v;
	}

	virtual std::streamsize xsputn(const char *p, std::streamsize n) {
		loglock.lock();
		m_string.append(p, p + n);

		int pos = 0;
		while (pos != std::string::npos) {
			pos = m_string.find('\n');
			if (pos != std::string::npos) {
				std::string tmp(m_string.begin(), m_string.begin() + pos);
			//	QScrollBar * sb = log_window->verticalScrollBar();
				//bool atMax = (sb->value() == sb->maximum()) ? true : false;
				log_window->setTextColor(msgColor);
				log_window->append(QString::fromStdString(m_string.c_str()));
		//		/*if (atMax)*/ sb->setValue(sb->maximum());
				m_string.erase(m_string.begin(), m_string.begin() + pos + 1);
			}
		}
		loglock.unlock();
		return n;
	}

private:
	std::ostream &m_stream;
	std::streambuf *m_old_buf;
	std::string m_string;
	std::mutex loglock;

	QColor msgColor;

	QTextEdit* log_window;
};

#endif