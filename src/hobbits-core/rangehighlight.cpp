#include "rangehighlight.h"

RangeHighlight::RangeHighlight(QString category, QString label, Range range, QColor color, QList<RangeHighlight> children) :
    m_category(category),
    m_label(label),
    m_range(range),
    m_color(color),
    m_children(children)
{

}

RangeHighlight::RangeHighlight(QString category, QString label, QList<RangeHighlight> children, QColor color) :
    m_category(category),
    m_label(label),
    m_color(color),
    m_children(children)
{
    std::sort(m_children.begin(), m_children.end());
    if (m_children.isEmpty()) {
        return;
    }
    m_range = Range(m_children.first().range().start(), m_children.last().range().end());
}

bool operator<(const RangeHighlight &a, const RangeHighlight &b)
{
    return a.range() < b.range();
}

QString RangeHighlight::label() const
{
    return m_label;
}

QString RangeHighlight::category() const
{
    return m_category;
}

Range RangeHighlight::range() const
{
    return m_range;
}

QColor RangeHighlight::color() const
{
    return m_color;
}

QList<RangeHighlight> RangeHighlight::children() const
{
    return m_children;
}

QList<RangeHighlight> RangeHighlight::allDescendants() const
{
    QList<RangeHighlight> all;
    for (auto child : children()) {
        all.append(child);
        all.append(child.allDescendants());
    }
    return all;
}

const QString VERSION_1 = "RangeHighlight v1";
const QString VERSION_2 = "RangeHighlight v2";
QDataStream& operator<<(QDataStream& stream, const RangeHighlight& highlight)
{
    stream << VERSION_2;
    stream << highlight.category();
    stream << highlight.label();
    stream << highlight.range();
    stream << highlight.color();
    stream << highlight.children();
    return stream;
}

QDataStream& operator>>(QDataStream& stream, RangeHighlight& highlight)
{
    QString version;
    stream >> version;
    if (version == VERSION_1 || version == VERSION_2) {
        stream >> highlight.m_category;
        stream >> highlight.m_label;
        stream >> highlight.m_range;
        stream >> highlight.m_color;
        if (version == VERSION_2) {
            stream >> highlight.m_children;
        }
        return stream;
    }
    else {
        stream.setStatus(QDataStream::Status::ReadCorruptData);
        return stream;
    }
}
