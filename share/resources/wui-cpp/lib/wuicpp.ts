/// <reference path="../jqwidgets-ts/jqwidgets.d.ts" />
/// <reference path="../jqwidgets-ts/jquery.d.ts" />

import axios from 'axios';

export module wuicpp {
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
        private _grid_data = new Map<string, Array<CellValue>>();
        private _server_ip: string;
        private _container_creators = new Array<{ (): void; }>();

        constructor() {
            this._server_ip = window.location.href.substring(0, window.location.href.length - 1); // remove trailing /

            let this_obj = this;
            jqwidgets.createInstance(
                '#start-btn',
                'jqxButton',
                {
                    width: '120px',
                    height: '35px',
                    theme: 'bootstrap'
                }).addEventHandler('click',
                    function () {
                        let loading = document.createElement('div');
                        loading.id = 'ui-loader';
                        document.getElementById('wui-controls').appendChild(loading);
                        let loading_ui = jqwidgets.createInstance(
                            '#' + loading.id,
                            'jqxLoader', {
                            width: '100px',
                            height: '100px'
                        }) as jqwidgets.jqxLoader;

                        loading_ui.open(0, 0);

                        this_obj.stopUpdateTimer();
                        axios.get(this_obj._server_ip + '/get_ui', {
                            responseType: 'json'
                        }).then(function (response) {
                            this_obj.createUI(response.data);
                            loading_ui.close();
                        }
                        ).catch(function (error) {
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
                    function () {
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
                function (event) {
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
        //                 axios.get(this._server_ip + '/get_value/12', {
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

        getUIContainer(container_name: string): HTMLElement {
            let root = document.getElementById('wui-container');
            if (container_name.length === 0) {
                return root;
            }
            else {
                let paths = container_name.split('/');
                paths.shift(); // remove first empty element
                let current_path = 'wui-container';
                let last = root;
                paths.forEach(sub_path => {
                    current_path = current_path + '-' + sub_path;
                    let current = document.getElementById(current_path);
                    if (current === null) {
                        current = document.createElement('div');
                        current.id = current_path;
                        last.appendChild(current);
                    }
                    last = current;
                });
                return last;
            }
        }

        clearUIContainer(container_name: string) {
            let container = this.getUIContainer(container_name);
            while (container.firstChild) {
                container.removeChild(container.firstChild);
            }
        }

        createUI(description) {
            this.clearUIContainer("");
            this._grid_data.clear();
            this._update_callbacks = [];
            description.widgets.forEach(widget => {
                if (widget.type === 'slider') {
                    this.createSlider(widget);
                }
                else if (widget.type == 'label') {
                    this.createLabel(widget);
                }
                else if (widget.type == 'tabs') {
                    this.createTabs(widget);
                }
                else if (widget.type == 'range') {
                    this.createRange(widget);
                }
                else if (widget.type == 'progress_bar') {
                    this.createProgressBar(widget);
                }
                else if (widget.type == 'button') {
                    this.createButton(widget);
                }
                else if (widget.type == 'switch') {
                    this.createSwitch(widget);
                }
                else if (widget.type == 'combobox') {
                    this.createComboBox(widget);
                }
                else {
                    console.log("Unknown widget type:", widget.type);
                }
            });
            this._container_creators.forEach(creator => {
                creator();
            })
            this.startUpdateTimer();
        }

        createSlider(widget: any) {
            let container = this.getUIContainer(widget.container);
            let slider_container = document.createElement('div');
            slider_container.style.width = '100%';
            slider_container.style.height = '100px';
            slider_container.style.display = 'flex';
            slider_container.setAttribute('widget-name', widget.name);

            let slider_label = document.createElement('input');
            slider_label.readOnly = true;
            slider_label.value = widget.name;
            slider_label.id = 'wui-slider-label-' + widget.id;
            slider_label.style.margin = 'auto auto auto 0';
            slider_container.appendChild(slider_label);

            let slider = document.createElement('div');
            slider.id = 'wui-slider-' + widget.id;
            slider_container.appendChild(slider);

            container.appendChild(slider_container);

            jqwidgets.createInstance(
                '#' + slider_label.id,
                'jqxInput', {
                width: '18%',
                height: '30%'
            });

            let this_obj = this;
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
                tickLabelFormatFunction: function (value) {
                    return Math.round(value * 1000000) / 1000000;
                },
                theme: 'bootstrap'
            }).addEventHandler('change', function (event) {
                var value = event.args.value;
                axios.post(this_obj._server_ip + '/set_value', {
                    id: widget.id,
                    value: value
                }, {
                    responseType: 'text'
                }).catch(function (error) {
                    console.log(error);
                });
            });
        }

        createRange(widget: any) {
            let container = this.getUIContainer(widget.container);
            let range_container = document.createElement('div');
            range_container.style.width = '100%';
            range_container.style.height = '100px';
            range_container.style.display = 'flex';
            range_container.setAttribute('widget-name', widget.name);

            let range_label = document.createElement('input');
            range_label.readOnly = true;
            range_label.value = widget.name;
            range_label.id = 'wui-range-label-' + widget.id;
            range_label.style.margin = 'auto auto auto 0';
            range_container.appendChild(range_label);

            let range = document.createElement('div');
            range.id = 'wui-range-' + widget.id;
            range_container.appendChild(range);

            container.appendChild(range_container);

            jqwidgets.createInstance(
                '#' + range_label.id,
                'jqxInput', {
                width: '18%',
                height: '30%'
            });

            let this_obj = this;
            let min = Number(widget.options.min);
            let max = Number(widget.options.max);
            let bounds = widget.options.default.split('/');
            jqwidgets.createInstance(
                '#' + range.id,
                'jqxRangeSelector', {
                width: '70%',
                height: '33%',
                // padding: 0,
                range: { from: Number(bounds[0]), to: Number(bounds[1]) },
                min: min,
                max: max,
                majorTicksInterval: (max - min) / 10,
                minorTicksInterval: (max - min) / 1000,
                labelsFormatFunction: function (value) {
                    return Math.round(value * 1000000) / 1000000;
                },
                markersFormatFunction: function (value) {
                    return Math.round(value * 1000000) / 1000000;
                },
                theme: 'bootstrap'
            }).addEventHandler('change', function (event) {
                axios.post(this_obj._server_ip + '/set_value', {
                    id: widget.id,
                    lower_bound: event.args.from,
                    upper_bound: event.args.to
                }, {
                    responseType: 'text'
                }).catch(function (error) {
                    console.log(error);
                });
            });
        }

        createProgressBar(widget: any) {
            let container = this.getUIContainer(widget.container);
            let pb_container = document.createElement('div');
            pb_container.style.width = '100%';
            pb_container.style.height = '60px';
            pb_container.style.display = 'flex';
            pb_container.setAttribute('widget-name', widget.name);

            let pb_label = document.createElement('input');
            pb_label.readOnly = true;
            pb_label.value = widget.name;
            pb_label.id = 'wui-progress_bar-label-' + widget.id;
            pb_label.style.margin = 'auto auto auto 0';
            pb_container.appendChild(pb_label);

            let progress_bar = document.createElement('div');
            progress_bar.id = 'wui-progress_bar-' + widget.id;
            progress_bar.style.top = '10px';
            pb_container.appendChild(progress_bar);

            container.appendChild(pb_container);

            jqwidgets.createInstance(
                '#' + pb_label.id,
                'jqxInput', {
                width: '18%',
                height: '30px'
            });

            let this_obj = this;
            let pb = jqwidgets.createInstance(
                '#' + progress_bar.id,
                'jqxProgressBar', {
                width: '80%',
                height: '40px',
                showText: true,
                animationDuration: 0,
                theme: 'bootstrap'
            }) as jqwidgets.jqxProgressBar;

            this._update_callbacks.push(function () {
                axios.post(this_obj._server_ip + '/get_value', {
                    id: widget.id
                }, {
                    responseType: 'text'
                })
                    .then(function (response) {
                        pb.actualValue(Number(response.data));
                    })
                    .catch(function (error) {
                        console.log(error);
                    });
            });
        }

        createButton(widget: any) {
            let container = this.getUIContainer(widget.container);
            let button = document.createElement('input');
            button.type = 'button';
            button.value = widget.name;
            button.id = 'wui-button-' + widget.id;
            button.setAttribute('widget-name', widget.name);
            container.appendChild(button);

            let this_obj = this;
            jqwidgets.createInstance(
                '#' + button.id,
                'jqxButton',
                {
                    width: '120px',
                    height: '35px',
                    theme: 'bootstrap'
                }).addEventHandler('click',
                    function () {
                        axios.post(this_obj._server_ip + '/set_value', {
                            id: widget.id,
                            state: true
                        }, {
                            responseType: 'text'
                        }).catch(function (error) {
                            console.log(error);
                        });
                    }
                );
        }

        createSwitch(widget: any) {
            let container = this.getUIContainer(widget.container);
            let switch_container = document.createElement('div');
            switch_container.style.width = '350px';
            switch_container.style.height = '60px';
            switch_container.style.display = 'flex';
            switch_container.setAttribute('widget-name', widget.name);

            let switch_label = document.createElement('input');
            switch_label.readOnly = true;
            switch_label.value = widget.name;
            switch_label.id = 'wui-switch_btn-label-' + widget.id;
            switch_label.style.margin = 'auto auto auto 0';
            switch_container.appendChild(switch_label);

            let switch_btn = document.createElement('div');
            switch_btn.id = 'wui-switch_btn-' + widget.id;
            switch_btn.style.margin = 'auto';
            switch_container.appendChild(switch_btn);

            container.appendChild(switch_container);

            jqwidgets.createInstance(
                '#' + switch_label.id,
                'jqxInput', {
                width: '180px',
                height: '30px'
            });

            let this_obj = this;
            jqwidgets.createInstance(
                '#' + switch_btn.id,
                'jqxSwitchButton',
                {
                    width: '120px',
                    height: '35px',
                    checked: widget.options.default == "1",
                    theme: 'bootstrap'
                }).addEventHandler('change',
                    function (event) {
                        axios.post(this_obj._server_ip + '/set_value', {
                            id: widget.id,
                            state: event.args.checked
                        }, {
                            responseType: 'text'
                        }).catch(function (error) {
                            console.log(error);
                        });
                    }
                );
        }

        createComboBox(widget: any) {
            let container = this.getUIContainer(widget.container);
            let combobox_container = document.createElement('div');
            combobox_container.style.width = '350px';
            combobox_container.style.height = '60px';
            combobox_container.style.display = 'flex';
            combobox_container.setAttribute('widget-name', widget.name);

            let combobox_label = document.createElement('input');
            combobox_label.readOnly = true;
            combobox_label.value = widget.name;
            combobox_label.id = 'wui-combobox-label-' + widget.id;
            combobox_label.style.margin = 'auto auto auto 0';
            combobox_container.appendChild(combobox_label);

            let combobox = document.createElement('div');
            combobox.id = 'wui-combobox-' + widget.id;
            combobox.style.margin = 'auto';
            combobox_container.appendChild(combobox);

            container.appendChild(combobox_container);

            jqwidgets.createInstance(
                '#' + combobox_label.id,
                'jqxInput', {
                width: '180px',
                height: '30px'
            });

            let this_obj = this;
            jqwidgets.createInstance(
                '#' + combobox.id,
                'jqxComboBox',
                {
                    width: '120px',
                    height: '35px',
                    source: widget.options.entries,
                    selectedIndex: widget.options.default,
                    theme: 'bootstrap'
                }).addEventHandler('select',
                    function (event) {
                        axios.post(this_obj._server_ip + '/set_value', {
                            id: widget.id,
                            index: event.args.index
                        }, {
                            responseType: 'text'
                        }).catch(function (error) {
                            console.log(error);
                        });
                    }
                );
        }

        createLabel(widget: any) {
            this.createGrid(widget.container);

            let cell_value = new CellValue(widget.name, '');
            this._grid_data[widget.container].push(cell_value);
            this.updateGrid(widget.container);

            let this_obj = this;
            this._update_callbacks.push(function () {
                axios.post(this_obj._server_ip + '/get_value', {
                    id: widget.id
                }, {
                    responseType: 'text'
                })
                    .then(function (response) {
                        cell_value.value = widget.options.prefix + response.data + widget.options.suffix;
                    })
                    .catch(function (error) {
                        console.log(error);
                    }).then(function () {
                        this_obj.updateGrid(widget.container);
                    });
            });
        }

        createTabs(widget: any) {
            let container = this.getUIContainer(widget.container);
            let tabs_container = document.createElement('div');
            tabs_container.id = container.id + '-' + widget.id;
            tabs_container.setAttribute('widget-name', widget.name);
            container.appendChild(tabs_container);

            let tab_list = document.createElement('ul');
            tabs_container.appendChild(tab_list);

            let first = true;
            widget.tabs.forEach(tab => {
                let tab_element = document.createElement('li');
                tab_element.innerText = tab;
                if (first) {
                    tab_element.style.marginLeft = '20px';
                    first = false;
                }
                tab_list.appendChild(tab_element);
            });

            this._container_creators.push(function () {
                jqwidgets.createInstance(
                    '#' + tabs_container.id,
                    'jqxTabs'
                );
            })
        }

        updateGrid(container_name: string) {
            let container = this.getUIContainer(container_name);
            let grid_id = container.id + '-' + 'table-values';
            let grid = $("#" + grid_id) as any;
            grid.jqxGrid('updatebounddata', 'cells');
        }

        createGrid(container_name: string) {
            let container = this.getUIContainer(container_name);
            let grid_id = container.id + '-' + 'table-values';
            if (document.getElementById(grid_id) === null) {
                // Create the grid when the first value is declared
                this._grid_data[container_name] = new Array<CellValue>();
                let grid = document.createElement('div');
                grid.id = grid_id;
                container.appendChild(grid);

                let source = {
                    datatype: "json",
                    datafields: [{
                        name: 'name'
                    },
                    {
                        name: 'value'
                    }
                    ],
                    localdata: this._grid_data[container_name]
                };

                jqwidgets.createInstance(
                    '#' + grid_id,
                    'jqxGrid',
                    {
                        source: source,
                        width: Number(container.offsetWidth - 4) + 'px',
                        columnsresize: true,
                        autoheight: true,
                        filterable: true,
                        columns: [{
                            text: 'Variable',
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
            }
        }
    }
}


export function createWuiApp(): wuicpp.Application {
    return new wuicpp.Application();
}
