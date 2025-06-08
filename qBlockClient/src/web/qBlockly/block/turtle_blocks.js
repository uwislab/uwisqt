// It is used to define the blocks of turtle library.

const import_turtle = {
  init: function() {
    this.appendDummyInput('NAME')
      .appendField('导入turtle库');
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setColour(330);
  }
};
Blockly.common.defineBlocks({import_turtle: import_turtle});
                    
const turtle_move = {
  init: function() {
    this.appendValueInput('a')
    .setCheck('Number')
      .appendField(new Blockly.FieldDropdown([
          ['向前走', 'forward'],
          ['向后走', 'backward']
        ]), 'b');
    this.setInputsInline(false)
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setColour(330);
  }
};
Blockly.common.defineBlocks({turtle_move: turtle_move});
                    
const turtle_turn = {
  init: function() {
    this.appendValueInput('a')
    .setCheck('Number')
      .appendField(new Blockly.FieldDropdown([
          ['向右转↻', 'right'],
          ['向左转↺', 'left']
        ]), 'b');
    this.setInputsInline(false)
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setColour(330);
  }
};
Blockly.common.defineBlocks({turtle_turn: turtle_turn});

const turtle_pen = {
  init: function() {
    this.appendDummyInput('NAME')
      .appendField(new Blockly.FieldDropdown([
          ['抬起画笔', 'penup'],
          ['放下画笔', 'pendown']
        ]), 'b');
    this.setInputsInline(false)
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setColour(330);
  }
};
Blockly.common.defineBlocks({turtle_pen: turtle_pen});
                    
const turtle_penshape = {
  init: function() {
    this.appendDummyInput('NAME')
      .appendField('画笔形状')
      .appendField(new Blockly.FieldDropdown([
          ['海龟', 'turtle'],
          ['圆形', 'circle'],
          ['箭头', 'classic'],
          ['正方形', 'square'],
          ['三角形', 'triangle']
        ]), 'b');
    this.setInputsInline(false)
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setColour(330);
  }
};
Blockly.common.defineBlocks({turtle_penshape: turtle_penshape});
                    
const turtle_pencolour = {
  init: function() {
    this.appendValueInput('a')
    .setCheck('String')
      .appendField('设置线条颜色');
    this.setInputsInline(false)
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setColour(330);
  }
};
Blockly.common.defineBlocks({turtle_pencolour: turtle_pencolour});

const turtle_fillcolour = {
  init: function() {
    this.appendValueInput('a')
    .setCheck('String')
      .appendField('设置填充颜色');
    this.setInputsInline(false)
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setColour(330);
  }
};
Blockly.common.defineBlocks({turtle_fillcolour: turtle_fillcolour});

const turtle_fillbegin = {
  init: function() {
    this.appendDummyInput('NAME')
      .appendField('开始填充');
    this.setInputsInline(false)
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setColour(330);
  }
};
Blockly.common.defineBlocks({turtle_fillbegin: turtle_fillbegin});

const turtle_fillend = {
  init: function() {
    this.appendDummyInput('NAME')
      .appendField('填充结束');
    this.setInputsInline(false)
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setColour(330);
  }
};
Blockly.common.defineBlocks({turtle_fillend: turtle_fillend});

const turtle_drawcircle = {
  init: function() {
    this.appendDummyInput('NAME')
      .appendField('画一个半径为');
    this.appendValueInput('a')
    .setCheck('Number');
    this.appendDummyInput('NAME')
      .appendField('的圆');
    this.setInputsInline(true)
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setColour(330);
  }
};
Blockly.common.defineBlocks({turtle_drawcircle: turtle_drawcircle});

const turtle_goto = {
  init: function() {
    this.appendDummyInput('NAME')
      .appendField('移动到坐标为');
    this.appendValueInput('x')
    .setCheck('Number')
      .appendField('x=');
    this.appendValueInput('y')
    .setCheck('Number')
      .appendField(',y=');
    this.appendDummyInput('NAME')
      .appendField('的位置');
    this.setInputsInline(true)
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setColour(330);
  }
};
Blockly.common.defineBlocks({turtle_goto: turtle_goto});

const turtle_pensize = {
  init: function() {
    this.appendValueInput('a')
    .setCheck('Number')
      .appendField('设置画笔线条大小');
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setColour(330);
  }
};
Blockly.common.defineBlocks({turtle_pensize: turtle_pensize});

const turtle_dot = {
  init: function() {
    this.appendValueInput('a')
    .setCheck('Number')
      .appendField('画一个直径为');
    this.appendValueInput('b')
    .setCheck('String')
      .appendField('颜色为');
    this.appendDummyInput('NAME')
      .appendField('的圆点');
    this.setInputsInline(true)
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setColour(330);
  }
};
Blockly.common.defineBlocks({turtle_dot: turtle_dot});

const turtle_home = {
  init: function() {
    this.appendDummyInput('NAME')
      .appendField('画笔回到原点');
    this.setInputsInline(true)
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setColour(330);
  }
};
Blockly.common.defineBlocks({turtle_home: turtle_home});

const turtle_penhide = {
  init: function() {
    this.appendDummyInput('NAME')
      .appendField(new Blockly.FieldDropdown([
          ['隐藏', 'hide'],
          ['显示', 'show']
        ]), 'a')
      .appendField('画笔');
    this.setInputsInline(true)
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setColour(330);
  }
};
Blockly.common.defineBlocks({turtle_penhide: turtle_penhide});

const turtle_stamp = {
  init: function() {
    this.appendDummyInput('NAME')
      .appendField('放置画笔印章');
    this.setInputsInline(true)
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setColour(330);
  }
};
Blockly.common.defineBlocks({turtle_stamp: turtle_stamp});

const turtle_clear = {
  init: function() {
    this.appendDummyInput('NAME')
      .appendField('清空画布');
    this.setInputsInline(true)
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setColour(330);
  }
};
Blockly.common.defineBlocks({turtle_clear: turtle_clear});

const turtle_undo = {
  init: function() {
    this.appendDummyInput('NAME')
      .appendField('撤销上一个动作');
    this.setInputsInline(true)
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setColour(330);
  }
};
Blockly.common.defineBlocks({turtle_undo: turtle_undo});

const turtle_done = {
  init: function() {
    this.appendDummyInput('NAME')
      .appendField('启动事件循环');
    this.setInputsInline(true)
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setColour(330);
  }
};
Blockly.common.defineBlocks({turtle_done: turtle_done});

const turtle_write = {
  init: function() {
    this.appendValueInput('s')
    .setCheck('String')
      .appendField('写文本：');
    this.appendDummyInput('name')
      .appendField('字体名')
      .appendField(new Blockly.FieldTextInput('宋体'), 'name');
    this.appendValueInput('size')
    .setCheck('Number')
      .appendField('字体大小');
    this.appendDummyInput('type')
      .appendField('字体类型')
      .appendField(new Blockly.FieldTextInput('normal'), 'type');
    this.setInputsInline(true)
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setColour(330);
  }
};
Blockly.common.defineBlocks({turtle_write: turtle_write});

const turtle_speed = {
  init: function() {
    this.appendValueInput('a')
    .setCheck('Number')
      .appendField('设置画笔速度');
    this.setInputsInline(true)
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setColour(330);
  }
};
Blockly.common.defineBlocks({turtle_speed: turtle_speed});
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                                                          
                                        
                                            