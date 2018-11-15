/// <reference path="../jqwidgets-ts/jqwidgets.d.ts" />
/// <reference path="../jqwidgets-ts/jquery.d.ts" />

import axios from 'axios';

export module wuicpp {
    function removeSpaces(text: string): string {
        return text.replace(/ /g, "_");
    }

    class CellValue {
        constructor(name: string, value: string) {
            this.name = name;
            this.value = value;
        }

        name: string;
        value: string;
    }

    export class Application {
        private _update_callbacks = new Array<{ (): void; }>();
        private _update_timer: number = NaN;
        private _update_timer_period_ms = 1000;
        private _read_values = new Array<CellValue>();
        private _read_values_adapter: any;

        constructor() {
            let this_obj = this;
            jqwidgets.createInstance(
                '#start-btn',
                'jqxButton',
                {
                    width: '120px',
                    height: '35px',
                    theme: 'bootstrap'
                }).addEventHandler('click',
                    function() {
                        this_obj.stopUpdateTimer();
                        axios.get('http://localhost:8080/get_ui', {
                            responseType: 'json'
                        }).then(function(response) {
                            this_obj.createUI(response.data);
                        }
                        ).catch(function(error) {
                            alert('Got an error:' + error)
                        });
                    }
                );
            jqwidgets.createInstance(
                '#stop-btn',
                'jqxButton',
                {
                    width: '120px',
                    height: '35px',
                    theme: 'bootstrap'
                }).addEventHandler('click',
                    function() {
                        this_obj.stopUpdateTimer();
                    }
                );

            jqwidgets.createInstance(
                '#refresh-rate',
                'jqxNumberInput',
                {
                    width: '120px',
                    height: '35px',
                    spinButtons: true,
                    spinButtonsStep: 10,
                    groupSeparator: '',
                    promptChar: '',
                    min: 1,
                    decimalDigits: 0,
                    decimal: this._update_timer_period_ms,
                    symbol: 'ms',
                    symbolPosition: 'right'
                }
            ).addEventHandler('change',
                function(event) {
                    this_obj._update_timer_period_ms = Number(event.args.value);
                    if (this_obj.isUpdateTimerStarted()) {
                        this_obj.stopUpdateTimer();
                        this_obj.startUpdateTimer();
                    }
                });
        }
        // constructor() {
        //     let sampleData = [];
        //
        //     var settings = {
        //         title: "blah blah blah",
        //         description: "",
        //         // legend: "",
        //         padding: {
        //             left: 5,
        //             top: 5,
        //             right: 5,
        //             bottom: 5
        //         },
        //         titlePadding: {
        //             left: 90,
        //             top: 0,
        //             right: 0,
        //             bottom: 10
        //         },
        //         categoryAxis: {
        //             dataField: 'Time',
        //             showGridLines: true
        //         },
        //         colorScheme: 'scheme01',
        //         seriesGroups: [{
        //             type: 'line',
        //             columnsGapPercent: 30,
        //             seriesGapPercent: 0,
        //             valueAxis: {
        //                 unitInterval: 10,
        //                 description: 'Voltage'
        //             },
        //             series: [{
        //                 dataField: 'y1',
        //                 displayText: 'y1'
        //             }]
        //         }],
        //         animationDuration: 0,
        //         toolTipMoveDuration: 10,
        //     };
        //
        //     let chart = jqwidgets.createInstance(
        //         '#chartContainer',
        //         'jqxChart',
        //         settings) as jqwidgets.jqxChart;
        //
        //     let timer: number;
        //
        //     jqwidgets.createInstance(
        //         '#startChart',
        //         'jqxButton', {
        //             width: '120px',
        //             height: '35px',
        //             theme: 'bootstrap'
        //         }).addEventHandler('click', function() {
        //             let time = 0;
        //             sampleData = [];
        //             timer = setInterval(function() {
        //                 axios.get('http://localhost:8080/get_value/12', {
        //                     responseType: 'text'
        //                 })
        //                     .then(function(response) {
        //                         sampleData.push({
        //                             Time: time,
        //                             y1: response.data
        //                         });
        //                         chart.source = sampleData;
        //                         chart.update();
        //                         time += 1;
        //                     })
        //                     .catch(function(error) {
        //                         console.log(error);
        //                     });
        //             },
        //                 25);
        //         });
        //
        //     jqwidgets.createInstance(
        //         '#stopChart',
        //         'jqxButton', {
        //             width: '120px',
        //             height: '35px',
        //             theme: 'bootstrap'
        //         }).addEventHandler('click', function() {
        //             if (timer != null) {
        //                 clearInterval(timer);
        //             }
        //             // $('#chartContainer').jqxChart('saveAsPNG', 'myChart.png', getExportServer());
        //         });
        // }

        isUpdateTimerStarted(): boolean {
            return !isNaN(this._update_timer);
        }

        startUpdateTimer() {
            let this_obj = this;
            function process_callbacks() {
                this_obj._update_callbacks.forEach(callback => {
                    callback();
                });
            };
            process_callbacks();
            this._update_timer = setInterval(process_callbacks, this._update_timer_period_ms);
        }

        stopUpdateTimer() {
            if (this.isUpdateTimerStarted()) {
                clearInterval(this._update_timer);
                this._update_timer = NaN;
            }
        }

        getUIContainer(): HTMLElement {
            return document.getElementById('wui-container');
        }

        clearUIContainer() {
            let container = this.getUIContainer();
            while (container.firstChild) {
                container.removeChild(container.firstChild);
            }
        }

        createUI(description) {
            this.clearUIContainer();
            this._read_values = [];
            this._update_callbacks = [];
            description.widgets.forEach(widget => {
                if (widget.type === 'slider') {
                    this.createSlider(widget);
                }
                else if (widget.type == 'label') {
                    this.createLabel(widget);
                }
            });
            if (this._read_values.length > 0) {
                this.startUpdateTimer();
            }
        }

        createSlider(widget: any) {
            let widget_name = widget.id;
            let widget_id = removeSpaces(widget.id);
            let container = this.getUIContainer();
            let slider_container = document.createElement('div');
            slider_container.style.width = '100%';
            slider_container.style.height = '100px';
            slider_container.style.display = 'flex';

            let slider_label = document.createElement('input');
            slider_label.readOnly = true;
            slider_label.value = widget_name;
            slider_label.id = 'wui-slider-label-' + widget_id;
            slider_label.style.margin = 'auto auto auto 0';
            slider_container.appendChild(slider_label);

            let slider = document.createElement('div');
            slider.id = 'wui-slider-' + widget_id;
            slider_container.appendChild(slider);

            container.appendChild(slider_container);

            jqwidgets.createInstance(
                '#' + slider_label.id,
                'jqxInput', {
                    width: '18%',
                    height: '30%'
                });

            let min = Number(widget.options.min);
            let max = Number(widget.options.max);
            jqwidgets.createInstance(
                '#' + slider.id,
                'jqxSlider', {
                    width: '80%',
                    height: '100%',
                    value: Number(widget.options.default),
                    min: min,
                    max: max,
                    showTicks: true,
                    ticksFrequency: (max - min) / 5,
                    showMinorTicks: true,
                    minorTicksFrequency: (max - min) / 10,
                    step: (max - min) / 10,
                    showTickLabels: true,
                    showRange: true,
                    tooltip: true,
                    tickLabelFormatFunction: function(value) {
                        return Math.round(value * 1000000) / 1000000;
                    },
                    theme: 'bootstrap'
                }).addEventHandler('change', function(event) {
                    var value = event.args.value;
                    axios.post('http://localhost:8080/set_value', {
                        id: widget_name,
                        value: value
                    }, {
                            responseType: 'text'
                        }).catch(function(error) {
                            console.log(error);
                        });
                });
        }

        createLabel(widget: any) {
            this.createGrid();

            let cell_value = new CellValue(widget.id, '');
            this._read_values.push(cell_value);
            this.updateGrid();

            let this_obj = this;
            this._update_callbacks.push(function() {
                axios.post('http://localhost:8080/get_value', {
                    id: widget.id
                }, {
                        responseType: 'text'
                    })
                    .then(function(response) {
                        cell_value.value = widget.options.prefix + response.data + widget.options.postfix;
                    })
                    .catch(function(error) {
                        console.log(error);
                    }).then(function() {
                        this_obj.updateGrid();
                    });
            });
        }

        updateGrid() {
            jqwidgets.createInstance(
                '#wui-table-values',
                'jqxGrid',
                {
                    source: this._read_values_adapter
                });
        }

        createGrid() {
            if (document.getElementById('wui-table-values') === null) {
                // Create the grid when the first value is declared
                let container = this.getUIContainer();
                let grid = document.createElement('div');
                grid.id = 'wui-table-values';
                container.appendChild(grid);

                jqwidgets.createInstance(
                    '#wui-table-values',
                    'jqxGrid',
                    {
                        width: '100%',
                        columnsresize: true,
                        columns: [{
                            text: 'Name',
                            datafield: 'name',
                            width: '50%'
                        },
                        {
                            text: 'Value',
                            datafield: 'value',
                            width: '50%'
                        }
                        ]
                    });
                let source = {
                    datatype: "json",
                    datafields: [{
                        name: 'name'
                    },
                    {
                        name: 'value'
                    }
                    ],
                    localdata: this._read_values
                };
                this._read_values_adapter = new $.jqx.dataAdapter(source);
            }
        }
    }
}


export function createWuiApp(): wuicpp.Application {
    return new wuicpp.Application();
}
