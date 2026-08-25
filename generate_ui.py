import xml.etree.ElementTree as ET

def create_ui():
    ui = ET.Element("ui", {"version": "4.0"})
    
    # Class
    ET.SubElement(ui, "class").text = "MainWindow"
    
    # MainWindow
    mw = ET.SubElement(ui, "widget", {"class": "QMainWindow", "name": "MainWindow"})
    
    # Geometry
    geom = ET.SubElement(mw, "property", {"name": "geometry"})
    rect = ET.SubElement(geom, "rect")
    ET.SubElement(rect, "x").text = "0"
    ET.SubElement(rect, "y").text = "0"
    ET.SubElement(rect, "width").text = "950"
    ET.SubElement(rect, "height").text = "700"
    
    ET.SubElement(ET.SubElement(mw, "property", {"name": "windowTitle"}), "string").text = "Computer Graphics Lab"
    
    # Central Widget
    cw = ET.SubElement(mw, "widget", {"class": "QWidget", "name": "centralwidget"})
    
    # Main Layout (HBox)
    main_layout = ET.SubElement(cw, "layout", {"class": "QHBoxLayout", "name": "horizontalLayout"})
    
    # Left item: Canvas
    canvas_item = ET.SubElement(main_layout, "item")
    frame = ET.SubElement(canvas_item, "widget", {"class": "my_label", "name": "frame"})
    sp = ET.SubElement(frame, "property", {"name": "sizePolicy"})
    policy = ET.SubElement(sp, "sizepolicy", {"hsizetype": "Expanding", "vsizetype": "Expanding"})
    ET.SubElement(policy, "horstretch").text = "1"
    ET.SubElement(policy, "verstretch").text = "1"
    min_size = ET.SubElement(ET.SubElement(frame, "property", {"name": "minimumSize"}), "size")
    ET.SubElement(min_size, "width").text = "550"
    ET.SubElement(min_size, "height").text = "550"
    ET.SubElement(ET.SubElement(frame, "property", {"name": "frameShape"}), "enum").text = "QFrame::Box"
    ET.SubElement(ET.SubElement(frame, "property", {"name": "text"}), "string").text = ""
    
    # Right item: Controls (VBox)
    controls_item = ET.SubElement(main_layout, "item")
    controls_layout = ET.SubElement(controls_item, "layout", {"class": "QVBoxLayout", "name": "verticalLayoutControls"})
    ET.SubElement(ET.SubElement(controls_layout, "property", {"name": "spacing"}), "number").text = "10"
    ET.SubElement(ET.SubElement(controls_layout, "property", {"name": "sizeConstraint"}), "enum").text = "QLayout::SetFixedSize"
    
    # Global Settings Group
    global_item = ET.SubElement(controls_layout, "item")
    global_gb = ET.SubElement(global_item, "widget", {"class": "QGroupBox", "name": "groupBoxGlobal"})
    ET.SubElement(ET.SubElement(global_gb, "property", {"name": "title"}), "string").text = "Global Settings"
    global_layout = ET.SubElement(global_gb, "layout", {"class": "QHBoxLayout", "name": "horizontalLayoutGlobal"})
    
    lbl_grid = ET.SubElement(global_layout, "item")
    lbl_g_widget = ET.SubElement(lbl_grid, "widget", {"class": "QLabel", "name": "labelGrid"})
    ET.SubElement(ET.SubElement(lbl_g_widget, "property", {"name": "text"}), "string").text = "Grid Gaps:"
    
    spin_grid = ET.SubElement(global_layout, "item")
    spin_g_widget = ET.SubElement(spin_grid, "widget", {"class": "QSpinBox", "name": "spinBox"})
    ET.SubElement(ET.SubElement(spin_g_widget, "property", {"name": "minimum"}), "number").text = "5"
    ET.SubElement(ET.SubElement(spin_g_widget, "property", {"name": "value"}), "number").text = "15"
    
    btn_grid = ET.SubElement(global_layout, "item")
    btn_g_widget = ET.SubElement(btn_grid, "widget", {"class": "QPushButton", "name": "pushButton_2"})
    ET.SubElement(ET.SubElement(btn_g_widget, "property", {"name": "text"}), "string").text = "Apply Grid"
    
    # Tab Widget
    tab_item = ET.SubElement(controls_layout, "item")
    tab_widget = ET.SubElement(tab_item, "widget", {"class": "QTabWidget", "name": "tabWidget"})
    ET.SubElement(ET.SubElement(tab_widget, "property", {"name": "currentIndex"}), "number").text = "0"
    
    # ------------------ LINE TAB ------------------
    tab_line = ET.SubElement(tab_widget, "widget", {"class": "QWidget", "name": "tabLine"})
    ET.SubElement(ET.SubElement(tab_line, "attribute", {"name": "title"}), "string").text = "Line Drawing"
    line_layout = ET.SubElement(tab_line, "layout", {"class": "QVBoxLayout", "name": "verticalLayoutLine"})
    
    # Algorithm Group
    la_item = ET.SubElement(line_layout, "item")
    la_gb = ET.SubElement(la_item, "widget", {"class": "QGroupBox", "name": "groupBoxLineAlgo"})
    ET.SubElement(ET.SubElement(la_gb, "property", {"name": "title"}), "string").text = "Algorithm Selection"
    la_vbox = ET.SubElement(la_gb, "layout", {"class": "QVBoxLayout", "name": "vboxLineAlgo"})
    
    cmb_la = ET.SubElement(la_vbox, "item")
    cmb_la_widget = ET.SubElement(cmb_la, "widget", {"class": "QComboBox", "name": "comboLineAlgo"})
    for text in ["DDA Algorithm", "Bresenham Algorithm", "All (Overlap)"]:
        item = ET.SubElement(cmb_la_widget, "item")
        ET.SubElement(ET.SubElement(item, "property", {"name": "text"}), "string").text = text

    # Controls Group
    lc_item = ET.SubElement(line_layout, "item")
    lc_gb = ET.SubElement(lc_item, "widget", {"class": "QGroupBox", "name": "groupBoxLineControls"})
    ET.SubElement(ET.SubElement(lc_gb, "property", {"name": "title"}), "string").text = "Controls"
    lc_hbox = ET.SubElement(lc_gb, "layout", {"class": "QHBoxLayout", "name": "hboxLineControls"})
    
    cmb_lp = ET.SubElement(lc_hbox, "item")
    cmb_lp_widget = ET.SubElement(cmb_lp, "widget", {"class": "QComboBox", "name": "comboLinePoint"})
    for text in ["Select Point 1", "Select Point 2"]:
        item = ET.SubElement(cmb_lp_widget, "item")
        ET.SubElement(ET.SubElement(item, "property", {"name": "text"}), "string").text = text

    # Performance Group
    lp_item = ET.SubElement(line_layout, "item")
    lp_gb = ET.SubElement(lp_item, "widget", {"class": "QGroupBox", "name": "groupBoxLinePerf"})
    ET.SubElement(ET.SubElement(lp_gb, "property", {"name": "title"}), "string").text = "Performance"
    lp_vbox = ET.SubElement(lp_gb, "layout", {"class": "QVBoxLayout", "name": "vboxLinePerf"})
    
    for name, text in [("dda_time_label", "DDA Time: -"), ("bresenham_time_label", "Bresenham Time: -")]:
        item = ET.SubElement(lp_vbox, "item")
        lbl = ET.SubElement(item, "widget", {"class": "QLabel", "name": name})
        ET.SubElement(ET.SubElement(lbl, "property", {"name": "text"}), "string").text = text
        font = ET.SubElement(ET.SubElement(lbl, "property", {"name": "font"}), "font")
        ET.SubElement(font, "weight").text = "75"
        ET.SubElement(font, "bold").text = "true"

    # Actions Group
    lac_item = ET.SubElement(line_layout, "item")
    lac_gb = ET.SubElement(lac_item, "widget", {"class": "QGroupBox", "name": "groupBoxLineActions"})
    ET.SubElement(ET.SubElement(lac_gb, "property", {"name": "title"}), "string").text = "Actions"
    lac_grid = ET.SubElement(lac_gb, "layout", {"class": "QGridLayout", "name": "gridLineActions"})
    
    for row, col, name, text in [(0,0,"clear","Clear All"), (0,1,"pushButton_3","New Line"), (1,0,"draw_line","Draw Line"), (1,1,"pushButton_4","Draw Both")]:
        item = ET.SubElement(lac_grid, "item", {"row": str(row), "column": str(col)})
        btn = ET.SubElement(item, "widget", {"class": "QPushButton", "name": name})
        ET.SubElement(ET.SubElement(btn, "property", {"name": "text"}), "string").text = text

    # Spacer
    ET.SubElement(line_layout, "item").append(ET.fromstring('<spacer name="spacerLine"><property name="orientation"><enum>Qt::Vertical</enum></property><property name="sizeHint" stdset="0"><size><width>20</width><height>40</height></size></property></spacer>'))
    
    
    # ------------------ CIRCLE TAB ------------------
    tab_circle = ET.SubElement(tab_widget, "widget", {"class": "QWidget", "name": "tabCircle"})
    ET.SubElement(ET.SubElement(tab_circle, "attribute", {"name": "title"}), "string").text = "Circle Drawing"
    circle_layout = ET.SubElement(tab_circle, "layout", {"class": "QVBoxLayout", "name": "verticalLayoutCircle"})
    
    # Algorithm Group
    ca_item = ET.SubElement(circle_layout, "item")
    ca_gb = ET.SubElement(ca_item, "widget", {"class": "QGroupBox", "name": "groupBoxCircleAlgo"})
    ET.SubElement(ET.SubElement(ca_gb, "property", {"name": "title"}), "string").text = "Algorithm Selection"
    ca_vbox = ET.SubElement(ca_gb, "layout", {"class": "QVBoxLayout", "name": "vboxCircleAlgo"})
    
    cmb_ca = ET.SubElement(ca_vbox, "item")
    cmb_ca_widget = ET.SubElement(cmb_ca, "widget", {"class": "QComboBox", "name": "comboCircleAlgo"})
    for text in ["Polar Algorithm", "Midpoint (Bresenham) Algorithm", "Cartesian Algorithm", "All (Overlap)"]:
        item = ET.SubElement(cmb_ca_widget, "item")
        ET.SubElement(ET.SubElement(item, "property", {"name": "text"}), "string").text = text

    # Controls Group
    cc_item = ET.SubElement(circle_layout, "item")
    cc_gb = ET.SubElement(cc_item, "widget", {"class": "QGroupBox", "name": "groupBoxCircleControls"})
    ET.SubElement(ET.SubElement(cc_gb, "property", {"name": "title"}), "string").text = "Controls"
    cc_hbox = ET.SubElement(cc_gb, "layout", {"class": "QHBoxLayout", "name": "hboxCircleControls"})
    
    cmb_cp = ET.SubElement(cc_hbox, "item")
    cmb_cp_widget = ET.SubElement(cmb_cp, "widget", {"class": "QComboBox", "name": "comboCirclePoint"})
    for text in ["Select Center", "Select Radius"]:
        item = ET.SubElement(cmb_cp_widget, "item")
        ET.SubElement(ET.SubElement(item, "property", {"name": "text"}), "string").text = text
        
    rad_lbl = ET.SubElement(cc_hbox, "item")
    ET.SubElement(ET.SubElement(ET.SubElement(rad_lbl, "widget", {"class": "QLabel", "name": "labelRadius"}), "property", {"name": "text"}), "string").text = "Radius:"
    
    rad_spin = ET.SubElement(cc_hbox, "item")
    spin_rad_widget = ET.SubElement(rad_spin, "widget", {"class": "QSpinBox", "name": "spinBoxRadius"})
    ET.SubElement(ET.SubElement(spin_rad_widget, "property", {"name": "maximum"}), "number").text = "1000"

    # Performance Group
    cp_item = ET.SubElement(circle_layout, "item")
    cp_gb = ET.SubElement(cp_item, "widget", {"class": "QGroupBox", "name": "groupBoxCirclePerf"})
    ET.SubElement(ET.SubElement(cp_gb, "property", {"name": "title"}), "string").text = "Performance"
    cp_vbox = ET.SubElement(cp_gb, "layout", {"class": "QVBoxLayout", "name": "vboxCirclePerf"})
    
    for name, text in [("lblPolarTime", "Polar Time: -"), ("lblMidpointTime", "Midpoint Time: -"), ("lblCartesianTime", "Cartesian Time: -")]:
        item = ET.SubElement(cp_vbox, "item")
        lbl = ET.SubElement(item, "widget", {"class": "QLabel", "name": name})
        ET.SubElement(ET.SubElement(lbl, "property", {"name": "text"}), "string").text = text
        font = ET.SubElement(ET.SubElement(lbl, "property", {"name": "font"}), "font")
        ET.SubElement(font, "weight").text = "75"
        ET.SubElement(font, "bold").text = "true"

    # Actions Group
    cac_item = ET.SubElement(circle_layout, "item")
    cac_gb = ET.SubElement(cac_item, "widget", {"class": "QGroupBox", "name": "groupBoxCircleActions"})
    ET.SubElement(ET.SubElement(cac_gb, "property", {"name": "title"}), "string").text = "Actions"
    cac_grid = ET.SubElement(cac_gb, "layout", {"class": "QGridLayout", "name": "gridCircleActions"})
    
    for row, col, name, text in [(0,0,"btnClearCircle","Clear Circle"), (0,1,"btnAnimateCircle","Animate Symmetry")]:
        item = ET.SubElement(cac_grid, "item", {"row": str(row), "column": str(col)})
        btn = ET.SubElement(item, "widget", {"class": "QPushButton", "name": name})
        ET.SubElement(ET.SubElement(btn, "property", {"name": "text"}), "string").text = text

    # Spacer
    ET.SubElement(circle_layout, "item").append(ET.fromstring('<spacer name="spacerCircle"><property name="orientation"><enum>Qt::Vertical</enum></property><property name="sizeHint" stdset="0"><size><width>20</width><height>40</height></size></property></spacer>'))
    
    # ------------------ END TABS ------------------
    
    ET.SubElement(controls_layout, "item").append(ET.fromstring('<spacer name="spacerGlobal"><property name="orientation"><enum>Qt::Vertical</enum></property><property name="sizeHint" stdset="0"><size><width>20</width><height>40</height></size></property></spacer>'))

    
    # MenuBar & StatusBar
    mb = ET.SubElement(mw, "widget", {"class": "QMenuBar", "name": "menubar"})
    ET.SubElement(ET.SubElement(mb, "property", {"name": "geometry"}), "rect").append(ET.fromstring('<x>0</x><y>0</y><width>950</width><height>24</height>'))
    ET.SubElement(mw, "widget", {"class": "QStatusBar", "name": "statusbar"})
    
    # Custom Widgets
    cw_node = ET.SubElement(ui, "customwidgets")
    c_w = ET.SubElement(cw_node, "customwidget")
    ET.SubElement(c_w, "class").text = "my_label"
    ET.SubElement(c_w, "extends").text = "QLabel"
    ET.SubElement(c_w, "header", {"location": "global"}).text = "my_label.h"
    
    ET.SubElement(ui, "resources")
    ET.SubElement(ui, "connections")

    tree = ET.ElementTree(ui)
    tree.write("/Users/anubhabrakshit/Downloads/drawLine/mainwindow.ui", encoding="UTF-8", xml_declaration=True)

if __name__ == "__main__":
    create_ui()
