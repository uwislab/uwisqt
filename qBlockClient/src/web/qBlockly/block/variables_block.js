const variable_declare = {
    init: function() {
      this.appendValueInput('varInput')
        .appendField(new Blockly.FieldTextInput('myVar'), 'varName');
      this.setTooltip('');
      this.setHelpUrl('');
      this.setColour(330);
    }
  };
  Blockly.common.defineBlocks({variable_declare: variable_declare});

  const variable_get = {
    init: function() {

      this.varNames = [["", ""]];

      this.appendDummyInput('NAME')
        .appendField(new Blockly.FieldDropdown(this.allocateDropdown.bind(this)), "VAR");
      this.setOutput(true, null);
      this.setTooltip('');
      this.setHelpUrl('');
      this.setColour(330);
    },

    allocateDropdown: function(){
      return this.varNames;
    },

    allocateVariables: function(){
      var options = [];
      
      options.push(["", ""]);
      
      //Previous declaration
      
      let ptr = this.parentBlock_;
          
      while(ptr){
    
        switch(ptr.type){
          case 'variable_declare':
            options.push([ptr.getFieldValue('varName'), ptr.getFieldValue('varName')]);
          break;
    
          default:
    
          if(ptr){
            switch(ptr.type){
    
              case 'loop_for':
              case 'loop_range':
                options.push([ptr.getVar_, ptr.getVar_]);
                this.isInitialized_ = ptr.isInitialized_;
              break;
    
    
            }
          }
    
    
          break;
        }
        
        ptr = ptr.parentBlock_;
      }
    
      //Loop through to get function variables
      ptr = this.getSurroundParent();
    
      while(ptr){
    
        switch( ptr.getDataStr() ){
    
          case 'isFunc':
      
            if(ptr.funcParam_){
      
              //Loop through the function array to get the names of parameters
              for(var i = 0; i < ptr.funcParam_.length; ++i){
                options.push([ptr.funcParam_[i][3], ptr.funcParam_[i][3]]);
              }
      
            }
      
          break;
    
        }
    
        ptr = ptr.getSurroundParent();
    
      }
      
      this.paramNames_ = options;
      
  
  
    },


  };
  Blockly.common.defineBlocks({variable_get: variable_get});
                      