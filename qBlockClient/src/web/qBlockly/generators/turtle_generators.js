// This file is used to generate Python code for turtle blocks.

python.pythonGenerator.forBlock['import_turtle'] = function() {

  // TODO: Assemble python into the code variable.
  const code = 'import turtle\nmy_turtle=turtle.Turtle()\n';
  return code;
}

python.pythonGenerator.forBlock['turtle_move'] = function(block) {
  const dropdown_b = block.getFieldValue('b');
  // TODO: change Order.ATOMIC to the correct operator precedence strength
  const value_a = Blockly.Python.valueToCode(block, 'a', python.Order.ATOMIC);

  // TODO: Assemble python into the code variable.
  const code = "my_turtle." + dropdown_b + "(" + value_a + ")\n";
  return code;
}

python.pythonGenerator.forBlock['turtle_turn'] = function(block) {
  const dropdown_b = block.getFieldValue('b');
  // TODO: change Order.ATOMIC to the correct operator precedence strength
  const value_a = Blockly.Python.valueToCode(block, 'a', python.Order.ATOMIC);

  // TODO: Assemble python into the code variable.
  const code = "my_turtle." + dropdown_b + "(" + value_a + ")\n";
  return code;
}

python.pythonGenerator.forBlock['turtle_pen'] = function(block) {
  const dropdown_b = block.getFieldValue('b');

  // TODO: Assemble python into the code variable.
  const code = "my_turtle." + dropdown_b + "()\n";
  return code;
}

python.pythonGenerator.forBlock['turtle_penshape'] = function(block) {
  const dropdown_b = block.getFieldValue('b');

  // TODO: Assemble python into the code variable.
  const code = 'my_turtle.shape("' + dropdown_b + '")\n';
  return code;
}

python.pythonGenerator.forBlock['turtle_pencolour'] = function(block) {
  // TODO: change Order.ATOMIC to the correct operator precedence strength
  const value_a = Blockly.Python.valueToCode(block, 'a', python.Order.ATOMIC);

  // TODO: Assemble python into the code variable.
  const code = "my_turtle.color(" + value_a + ")\n";
  return code;
}

python.pythonGenerator.forBlock['turtle_fillcolour'] = function(block) {
  // TODO: change Order.ATOMIC to the correct operator precedence strength
  const value_a = Blockly.Python.valueToCode(block, 'a', python.Order.ATOMIC);

  // TODO: Assemble python into the code variable.
  const code = "my_turtle.fillcolor(" + value_a + ")\n";
  return code;
}

python.pythonGenerator.forBlock['turtle_fillbegin'] = function() {

  // TODO: Assemble python into the code variable.
  const code = 'my_turtle.begin_fill()\n';
  return code;
}

python.pythonGenerator.forBlock['turtle_fillend'] = function() {

  // TODO: Assemble python into the code variable.
  const code = 'my_turtle.end_fill()\n';
  return code;
}

python.pythonGenerator.forBlock['turtle_drawcircle'] = function(block) {
  // TODO: change Order.ATOMIC to the correct operator precedence strength
  const value_a = Blockly.Python.valueToCode(block, 'a', python.Order.ATOMIC);


  // TODO: Assemble python into the code variable.
  const code = "my_turtle.circle(" + value_a + ")\n";
  return code;
}

python.pythonGenerator.forBlock['turtle_goto'] = function(block) {
  // TODO: change Order.ATOMIC to the correct operator precedence strength
  const value_x = Blockly.Python.valueToCode(block, 'x', python.Order.ATOMIC);

  // TODO: change Order.ATOMIC to the correct operator precedence strength
  const value_y = Blockly.Python.valueToCode(block, 'y', python.Order.ATOMIC);


  // TODO: Assemble python into the code variable.
  const code = "my_turtle.goto(" + value_x + "," + value_y + ")\n";
  return code;
}

python.pythonGenerator.forBlock['turtle_pensize'] = function(block) {
  // TODO: change Order.ATOMIC to the correct operator precedence strength
  const value_a = Blockly.Python.valueToCode(block, 'a', python.Order.ATOMIC);

  // TODO: Assemble python into the code variable.
  const code = "my_turtle.pensize(" + value_a + ")\n";
  return code;
}

python.pythonGenerator.forBlock['turtle_dot'] = function(block) {
  // TODO: change Order.ATOMIC to the correct operator precedence strength
  const value_a = Blockly.Python.valueToCode(block, 'a', python.Order.ATOMIC);

  // TODO: change Order.ATOMIC to the correct operator precedence strength
  const value_b = Blockly.Python.valueToCode(block, 'b', python.Order.ATOMIC);


  // TODO: Assemble python into the code variable.
  const code = "my_turtle.dot(" + value_a + "," + value_b + ")\n";
  return code;
}

python.pythonGenerator.forBlock['turtle_home'] = function() {

  // TODO: Assemble python into the code variable.
  const code = 'my_turtle.home()\n';
  return code;
}

python.pythonGenerator.forBlock['turtle_penhide'] = function(block) {
  const dropdown_a = block.getFieldValue('a');

  // TODO: Assemble python into the code variable.
  const code = "my_turtle." + dropdown_a + "turtle()\n";
  return code;
}

python.pythonGenerator.forBlock['turtle_stamp'] = function() {

  // TODO: Assemble python into the code variable.
  const code = 'my_turtle.stamp()\n';
  return code;
}

python.pythonGenerator.forBlock['turtle_clear'] = function() {

  // TODO: Assemble python into the code variable.
  const code = 'my_turtle.clear()\n';
  return code;
}

python.pythonGenerator.forBlock['turtle_undo'] = function() {

  // TODO: Assemble python into the code variable.
  const code = 'my_turtle.undo()\n';
  return code;
}

python.pythonGenerator.forBlock['turtle_done'] = function() {

  // TODO: Assemble python into the code variable.
  const code = 'turtle.done()\n';
  return code;
}

python.pythonGenerator.forBlock['turtle_write'] = function(block) {
  // TODO: change Order.ATOMIC to the correct operator precedence strength
  const value_s = Blockly.Python.valueToCode(block, 's', python.Order.ATOMIC);

  const text_name = block.getFieldValue('name');

  // TODO: change Order.ATOMIC to the correct operator precedence strength
  const value_size = Blockly.Python.valueToCode(block, 'size', python.Order.ATOMIC);

  const text_type = block.getFieldValue('type');

  // TODO: Assemble python into the code variable.
  const code = "my_turtle.write(" + value_s + ',font=("'+text_name +'", '+ value_size+'," '+text_type+'"))\n';
  return code;
}

python.pythonGenerator.forBlock['turtle_speed'] = function(block) {
  // TODO: change Order.ATOMIC to the correct operator precedence strength
  const value_a = Blockly.Python.valueToCode(block, 'a', python.Order.ATOMIC);

  // TODO: Assemble python into the code variable.
  const code = "my_turtle.speed(" + value_a + ")\n";
  return code;
}