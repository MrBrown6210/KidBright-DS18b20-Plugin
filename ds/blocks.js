Blockly.Blocks["DS.temp"] = {
    init: function () {
        this.appendDummyInput()
            .appendField(Blockly.Msg.DS_TEXT_TEMP_TITLE);
        this.setOutput(true, 'float');
        this.setInputsInline(true)
        this.setPreviousStatement(false);
        this.setNextStatement(false);
        this.setColour(58);
        this.setTooltip(Blockly.Msg.DS_TEXT_TOOLTIP);
        this.setHelpUrl(Blockly.Msg.DS_TEXT_HELPURL);
    }
};