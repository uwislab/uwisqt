python.pythonGenerator.forBlock['variable_declare'] = function(block) {
    const text_varname = block.getFieldValue('varName');
    // TODO: change Order.ATOMIC to the correct operator precedence strength
    const value_varinput = Blockly.Python.valueToCode(block, 'varInput', python.Order.ATOMIC);
  
    // TODO: Assemble python into the code variable.
    var code = '';
    if (value_varinput) {
        code += text_varname +' = '+ value_varinput + '\n';
    } else {
        code += text_varname + '\n';
    }
    return code;
  }

python.pythonGenerator.forBlock['variable_get'] = function(block) {
    const dropdown_var = block.getFieldValue('VAR');
    
    // TODO: Assemble python into the code variable.
    const code = type;
    // TODO: Change Order.NONE to the correct operator precedence strength
    return [code, python.Order.NONE];
}