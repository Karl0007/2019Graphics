#include "Painter.h"

QStringList Painter::GetIDList()
{
	QStringList list;
	list << "Null";
	for (auto &i : m_hash) list << QString::number(i.first);
	return list;
}

void Painter::DrawAll()
{
	for (int i = 0; i < m_width*m_height*m_deep; i++) m_img[i] = 255;
	for (auto &x : m_hash) {
		x.second->Draw(m_img,m_width,m_height);
	}
	if (m_tmp) m_tmp->Draw(m_img,m_width,m_height);
	if (m_current) {
		Byte tmp[3];
		m_current->GetColor(tmp[0], tmp[1], tmp[2]);
		m_current->SetColor(255, 0, 0)->Draw(m_img, m_width, m_height);
		m_current->Translate(1, 0)->Draw(m_img, m_width, m_height);
		m_current->Translate(-2, 0)->Draw(m_img, m_width, m_height);
		m_current->Translate(1, 1)->Draw(m_img, m_width, m_height);
		m_current->Translate(0, -2)->Draw(m_img, m_width, m_height);
		m_current->Translate(0, 1)->SetColor(tmp[0], tmp[1], tmp[2]);
	}
}
