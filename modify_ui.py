import xml.etree.ElementTree as ET

tree = ET.parse('/Users/anubhabrakshit/Downloads/drawLine/mainwindow.ui')
root = tree.getroot()

# Find the main horizontal layout
main_layout = root.find(".//layout[@name='horizontalLayout']")
# The second item in horizontal layout is the verticalLayout
right_vbox_item = main_layout.findall("item")[1]
right_vbox = right_vbox_item.find("layout")

# We will extract groupBoxPoints, groupBoxAlgorithm, groupBoxPerformance, groupBoxActions
# and put them into a Line tab.
# We will leave groupBoxGrid and Clear/New Line actions outside if we want, or just put them all in Line tab
# Actually, let's put Points, Algorithm, Performance, Actions into Line Tab.
# Let's keep Grid Settings in the main verticalLayout, above the tabs.

items_to_move = []
for item in right_vbox.findall("item"):
    widget = item.find("widget")
    if widget is not None:
        name = widget.get("name")
        if name in ["groupBoxPoints", "groupBoxAlgorithm", "groupBoxPerformance", "groupBoxActions"]:
            items_to_move.append(item)

# Remove them from right_vbox
for item in items_to_move:
    right_vbox.remove(item)

# Create QTabWidget
tab_item = ET.Element("item")
tab_widget = ET.SubElement(tab_item, "widget", {"class": "QTabWidget", "name": "tabWidget"})
curr_idx = ET.SubElement(tab_widget, "property", {"name": "currentIndex"})
ET.SubElement(curr_idx, "number").text = "0"

# --- Line Tab ---
tab_line = ET.SubElement(tab_widget, "widget", {"class": "QWidget", "name": "tabLine"})
line_title = ET.SubElement(tab_line, "attribute", {"name": "title"})
ET.SubElement(line_title, "string").text = "Line"
line_layout = ET.SubElement(tab_line, "layout", {"class": "QVBoxLayout", "name": "verticalLayoutLine"})
for item in items_to_move:
    line_layout.append(item)
# Add spacer
spacer_item = ET.SubElement(line_layout, "item")
spacer = ET.SubElement(spacer_item, "spacer", {"name": "spacerLine"})
ET.SubElement(spacer, "property", {"name": "orientation"}).append(ET.Element("enum", text="Qt::Vertical"))
size_hint = ET.SubElement(spacer, "property", {"name": "sizeHint", "stdset": "0"})
size = ET.SubElement(size_hint, "size")
ET.SubElement(size, "width").text = "20"
ET.SubElement(size, "height").text = "40"

# --- Circle Tab ---
tab_circle = ET.SubElement(tab_widget, "widget", {"class": "QWidget", "name": "tabCircle"})
circle_title = ET.SubElement(tab_circle, "attribute", {"name": "title"})
ET.SubElement(circle_title, "string").text = "Circle"
circle_layout = ET.SubElement(tab_circle, "layout", {"class": "QVBoxLayout", "name": "verticalLayoutCircle"})

# Circle Algorithm Group
circ_algo_item = ET.SubElement(circle_layout, "item")
circ_algo_gb = ET.SubElement(circ_algo_item, "widget", {"class": "QGroupBox", "name": "groupBoxCircleAlgo"})
ET.SubElement(ET.SubElement(circ_algo_gb, "property", {"name": "title"}), "string").text = "Algorithm Selection"
circ_algo_vbox = ET.SubElement(circ_algo_gb, "layout", {"class": "QVBoxLayout", "name": "vboxCircAlgo"})

for name, text, checked in [("radioPolar", "Polar", True), ("radioMidpoint", "Midpoint", False), ("radioCartesian", "Cartesian", False)]:
    rad_item = ET.SubElement(circ_algo_vbox, "item")
    rad = ET.SubElement(rad_item, "widget", {"class": "QRadioButton", "name": name})
    ET.SubElement(ET.SubElement(rad, "property", {"name": "text"}), "string").text = text
    if checked:
        ET.SubElement(ET.SubElement(rad, "property", {"name": "checked"}), "bool").text = "true"

# Circle Performance Group
circ_perf_item = ET.SubElement(circle_layout, "item")
circ_perf_gb = ET.SubElement(circ_perf_item, "widget", {"class": "QGroupBox", "name": "groupBoxCirclePerf"})
ET.SubElement(ET.SubElement(circ_perf_gb, "property", {"name": "title"}), "string").text = "Performance"
circ_perf_vbox = ET.SubElement(circ_perf_gb, "layout", {"class": "QVBoxLayout", "name": "vboxCircPerf"})

for name, text in [("lblPolarTime", "Polar Time: -"), ("lblMidpointTime", "Midpoint Time: -"), ("lblCartesianTime", "Cartesian Time: -")]:
    lbl_item = ET.SubElement(circ_perf_vbox, "item")
    lbl = ET.SubElement(lbl_item, "widget", {"class": "QLabel", "name": name})
    font = ET.SubElement(ET.SubElement(lbl, "property", {"name": "font"}), "font")
    ET.SubElement(font, "weight").text = "75"
    ET.SubElement(font, "bold").text = "true"
    ET.SubElement(ET.SubElement(lbl, "property", {"name": "text"}), "string").text = text

# Circle Actions Group
circ_act_item = ET.SubElement(circle_layout, "item")
circ_act_gb = ET.SubElement(circ_act_item, "widget", {"class": "QGroupBox", "name": "groupBoxCircleActions"})
ET.SubElement(ET.SubElement(circ_act_gb, "property", {"name": "title"}), "string").text = "Actions"
circ_act_grid = ET.SubElement(circ_act_gb, "layout", {"class": "QGridLayout", "name": "gridCircActions"})

btn_anim_item = ET.SubElement(circ_act_grid, "item", {"row": "0", "column": "0"})
btn_anim = ET.SubElement(btn_anim_item, "widget", {"class": "QPushButton", "name": "btnAnimateCircle"})
ET.SubElement(ET.SubElement(btn_anim, "property", {"name": "text"}), "string").text = "Animate Symmetry"

# Spacer
spacer_circ_item = ET.SubElement(circle_layout, "item")
spacer_circ = ET.SubElement(spacer_circ_item, "spacer", {"name": "spacerCirc"})
ET.SubElement(spacer_circ, "property", {"name": "orientation"}).append(ET.Element("enum", text="Qt::Vertical"))
size_hint_c = ET.SubElement(spacer_circ, "property", {"name": "sizeHint", "stdset": "0"})
size_c = ET.SubElement(size_hint_c, "size")
ET.SubElement(size_c, "width").text = "20"
ET.SubElement(size_c, "height").text = "40"

# Insert tab widget into right_vbox, right after grid settings (which is at index 0 now)
right_vbox.insert(1, tab_item)

tree.write('/Users/anubhabrakshit/Downloads/drawLine/mainwindow.ui', encoding="UTF-8", xml_declaration=True)
