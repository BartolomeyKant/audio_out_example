import QtQml 2.14
import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14

import my.company.controller 1.0

Window {
	id: rootWindow

	width:800
	height:600
	visible: true

	title: "Voice instructor test application"
	color: "white"

	Item {
		anchors.fill: parent
		anchors.margins: 50

		Button {
			id: left

			width: 200
			height: 50

			anchors.left: parent.left
			anchors.verticalCenter: parent.verticalCenter

			text: "Кнопка слева"
			enabled: Controller.step == Controller.Left

			onClicked: {
				Controller.press_left()
			}
		}
		Button {
			id: right

			width: 200
			height: 50

			anchors.right: parent.right
			anchors.verticalCenter: parent.verticalCenter

			text: "Кнопка справа"
			enabled: Controller.step == Controller.Right

			onClicked: {
				Controller.press_right()
			}
		}
		Button {
			id: top

			width: 200
			height: 50

			anchors.top: parent.top
			anchors.horizontalCenter: parent.horizontalCenter

			text: "Кнопка сверху"
			enabled: Controller.step == Controller.Top

			onClicked: {
				Controller.press_top()
			}
		}
		Button {
			id: bottom

			width: 200
			height: 50

			anchors.bottom: parent.bottom
			anchors.horizontalCenter: parent.horizontalCenter

			text: "Кнопка снизу"
			enabled: Controller.step == Controller.Bottom

			onClicked: {
				Controller.press_bottom()
			}
		}
		Button {
			id: center

			width: 200
			height: 50

			anchors.centerIn: parent

			text: "Кнопка в центре"
			enabled: Controller.step == Controller.Start

			onClicked: {
				Controller.press_center()
			}
		}
	}
}
