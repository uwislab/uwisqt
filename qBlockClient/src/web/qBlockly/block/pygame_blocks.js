// It is used to define the blocks of pygame library.

const import_pygames = {
    init: function() {
      this.appendDummyInput('NAME')
        .appendField('导入pygame库');
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setTooltip('');
      this.setHelpUrl('');
      this.setColour(270);
    }
  };
  Blockly.common.defineBlocks({import_pygames: import_pygames});