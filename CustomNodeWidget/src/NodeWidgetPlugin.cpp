#include "Node.h"
#include "NodeWidgetPlugin.h"

#include <QtCore/QtPlugin>

NodeWidgetPlugin::NodeWidgetPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void NodeWidgetPlugin::initialize(QDesignerFormEditorInterface * /*core*/)
{
    if (initialized)
        return;

    initialized = true;
}

bool NodeWidgetPlugin::isInitialized() const
{
    return initialized;
}

QWidget *NodeWidgetPlugin::createWidget(QWidget *parent)
{
    return new Node(parent);
}

QString NodeWidgetPlugin::name() const
{
    return "Node";
}

QString NodeWidgetPlugin::group() const
{
    return "My Plugins";
}

QIcon NodeWidgetPlugin::icon() const
{
    return QIcon();
}

QString NodeWidgetPlugin::toolTip() const
{
    return QString();
}

QString NodeWidgetPlugin::whatsThis() const
{
    return QString();
}

bool NodeWidgetPlugin::isContainer() const
{
    return false;
}

QString NodeWidgetPlugin::domXml() const
{
    return "<widget class=\"Node\" name=\"Node\">\n"
        " <property name=\"geometry\">\n"
        "  <rect>\n"
        "   <x>0</x>\n"
        "   <y>0</y>\n"
        "   <width>100</width>\n"
        "   <height>100</height>\n"
        "  </rect>\n"
        " </property>\n"
        "</widget>\n";
}

QString NodeWidgetPlugin::includeFile() const
{
    return "Node.h";
}
