var devices;
var ws = new WebSocket('ws://raspberrypi:8080/');

function scanBLE() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            devices = JSON.parse(this.responseText);
            var table = document.getElementById('device-table');
            while (table.rows.length > 1) {
                table.deleteRow(1);
            }

            for (var i = 0; i < devices.length; i++) {
                var row = table.insertRow(i + 1);
                var nameCell = row.insertCell(0);
                var bdaddrCell = row.insertCell(1);
                var statusCell = row.insertCell(2);
                var uuidsCell = row.insertCell(3);
                var connectCell = row.insertCell(4);


                nameCell.innerHTML = devices[i].name;
                bdaddrCell.innerHTML = devices[i].bdaddr;
                uuidsCell.innerHTML = devices[i].UUIDs;
                statusCell.innerHTML = devices[i].connected ? 'Connected' : 'Not Connected';

                var connectButton = document.createElement("button");
                connectButton.innerHTML = "Connect";
                connectButton.addEventListener("click", function () {
                    var bdaddr_ble = this.dataset.bdaddr;
                    var toggleConnectionState = this.dataset.toggleConnectionState;
                    // console.log("Connecting to:______" + i);
                    connectToDevice(bdaddr_ble, toggleConnectionState);
                });
                connectButton.dataset.bdaddr = row.cells[1].innerHTML;
                connectButton.dataset.toggleConnectionState = row.cells[2].innerHTML;
                connectCell.appendChild(connectButton);
            }
        }
    };
    xhttp.open("GET", "http://raspberrypi/cgi-bin/gateway/do_discover_devices.py?scantime=3000", true);
    xhttp.send();
}

function connectToDevice(bdaddr_ble, toggleConnectionState) {
    // console.log("Connecting to: " + bdaddr_ble);
    // console.log("Connecting ?" + toggleConnectionState);
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            // console.log(this.responseText);
            result = JSON.parse(this.responseText);
            message(result_string(result.result));
            if (result.result == 0) {
                var table = document.getElementById("device-table");
                if (toggleConnectionState == "Not Connected") {
                    for (var i = 1; i < table.rows.length; i++) {
                        var bdaddr_cell = table.rows[i].cells[1];
                        if (bdaddr_cell.innerHTML === bdaddr_ble) {
                            var connected_cell = table.rows[i].cells[2];
                            connected_cell.innerHTML = "Connected";
                        }
                    }
                } else {
                    for (var i = 1; i < table.rows.length; i++) {
                        var bdaddr_cell = table.rows[i].cells[1];
                        if (bdaddr_cell.innerHTML === bdaddr_ble) {
                            var connected_cell = table.rows[i].cells[2];
                            connected_cell.innerHTML = "Not Connected";
                        }
                    }
                }
                onServiceDiscovery(bdaddr_ble)
            }
        }
    };

    var args = {};
    args.bdaddr = bdaddr_ble;
    var json = JSON.stringify(args);
    var target = "do_connect.py";
    if (toggleConnectionState != "Not Connected") {
        target = "do_connect.py";
        info = "disconnecting...";
    }
    var info = "connecting..";
    xhttp.open("PUT", "http://raspberrypi/cgi-bin/gateway/" + target, true);
    xhttp.setRequestHeader('Content-type', 'application/json; charset=utf-8');
    xhttp.send(json);
    message(info);
}
function onServiceDiscovery(bdaddr_ble) {
    message("performing service discovery");
    // console.log("TRUE");
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            services = JSON.parse(this.responseText);
            if (services.result !== undefined) {
                message("ERROR " + services.result);
            } else {
                // console.log(services);
                message("");
                var service_count = services.length;
                var table = document.getElementById('service-table');
                var tbody = table.querySelector('tbody');
                while (table.rows.length > 1) {
                    table.deleteRow(1);
                }
                services.forEach(function (service) {
                    var row = tbody.insertRow();
                    var uuidCell = row.insertCell(0);
                    uuidCell.textContent = service.UUID;

                    var handleCell = row.insertCell(1);
                    handleCell.textContent = service.handle;

                    // table of characteristics
                    var charCell = row.insertCell(2);
                    var charTable = document.createElement('table');
                    charTable.className = 'characteristics-table';
                    charTable.setAttribute('border', '1');

                    var charTableHeader = charTable.createTHead();
                    var charTableHeaderRow = charTableHeader.insertRow();
                    var charUUIDHeader = charTableHeaderRow.insertCell(0);
                    charUUIDHeader.textContent = 'UUID';
                    var charNotifyingHeader = charTableHeaderRow.insertCell(1);
                    charNotifyingHeader.textContent = 'Notifying';
                    var charPropsHeader = charTableHeaderRow.insertCell(2);
                    charPropsHeader.textContent = 'Properties';

                    var charTableBody = charTable.createTBody();

                    service.characteristics.forEach(function (characteristic) {
                        var charTableRow = charTableBody.insertRow();
                        var charUUIDCell = charTableRow.insertCell(0);
                        charUUIDCell.textContent = characteristic.UUID;
                        var charNotifyingCell = charTableRow.insertCell(1);
                        charNotifyingCell.textContent = characteristic.notifying ? 'Yes' : 'No';

                        var charActionsCell = charTableRow.insertCell(2);
                        var actions = [];
                        if (characteristic.properties.includes('read')) {
                            var readButton = document.createElement('button');
                            readButton.textContent = 'read';
                            readButton.addEventListener('click', function () {
                                testclick('read', bdaddr_ble, readButton.dataset.characteristic_handle);
                            });
                            readButton.dataset.characteristic_handle = characteristic.handle
                            charActionsCell.appendChild(readButton);
                            actions.push('read');
                        }
                        if (characteristic.properties.includes('write')) {
                            var writeButton = document.createElement('button');
                            writeButton.textContent = 'write';
                            writeButton.addEventListener('click', function () {
                                testclick('write');
                            });
                            charActionsCell.appendChild(writeButton);
                            actions.push('write');
                        }
                        if (characteristic.properties.includes('notify')) {
                            var notifyButton = document.createElement('button');
                            notifyButton.textContent = 'notify';
                            notifyButton.addEventListener('click', function () {
                                testclick('notify');
                            });
                            charActionsCell.appendChild(notifyButton);
                            actions.push('notify');
                        }
                        if (characteristic.properties.includes('indicate')) {
                            var indicateButton = document.createElement('button');
                            indicateButton.textContent = 'indicate';
                            indicateButton.addEventListener('click', function () {
                                testclick('indicate');
                            });
                            charActionsCell.appendChild(indicateButton);
                            actions.push('indicate');
                        }
                    });

                    // Loại bỏ dấu phẩy cuối cùng
                    charCell.appendChild(charTable);
                });

                message("ready");
            }
        }
    };
    xhttp.open("GET", "http://raspberrypi/cgi-bin/gateway/do_service_discovery.py?bdaddr=" + bdaddr_ble, true);
    xhttp.send();
}
function testclick(action, bdaddr, characteristic_handle) {
    if (action == "read") {
        readCharacteristic(bdaddr, characteristic_handle);
    }
    if (action == "write") {
        var value_hex// = stringToHex(alert("Vui lòng nhập dữ liệu"));
        writecharacteristic(bdaddr, characteristic_handle,"4f4e");
    }
}

function readCharacteristic(bdaddr, characteristic_handle) {
    console.log(bdaddr, characteristic_handle);
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            console.log(this.responseText);
            result = JSON.parse(this.responseText);
            // convert to byte array and store in the characteristic object
            message(result_string(result.result));
            if (result.result == 0) {
                alert("READING Characteristic -------------------- Value: " + result.value)
            }
        }
    };
    xhttp.open("GET", "http://raspberrypi/cgi-bin/gateway/do_read_characteristic.py" + "?bdaddr=" + bdaddr + "&handle=" + characteristic_handle, true);
    xhttp.send();
}
function writecharacteristic(bdaddr, characteristic_handle,value_hex){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            console.log(this.responseText);
            result = JSON.parse(this.responseText);
            // convert to byte array and store in the characteristic object
            message(result_string(result.result));
            if (result.result == 0) {
                alert("Writing Characteristic OK")
            }
        }
    };
    var args = {};
    args.bdaddr = bdaddr;
    args.handle = characteristic_handle;
    args.value = value_hex
    var json = JSON.stringify(args);
    console.log(json);
    xhttp.open("PUT", "http://raspberrypi/cgi-bin/gateway/do_write_characteristic.py", true);
    xhttp.setRequestHeader('Content-type', 'application/json; charset=utf-8');
    xhttp.send(json);
}


function message(text) {
    element('message').innerHTML = text;
    var xhttp = new XMLHttpRequest();
}
function element(id) {
    return document.getElementById(id);
}
function result_string(result) {
    if (result == 0) {
        return "OK";
    } else {
        return "ERROR " + result;
    }
}
var devices;
var ws = new WebSocket('ws://raspberrypi:8080/');

function scanBLE() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            devices = JSON.parse(this.responseText);
            var table = document.getElementById('device-table');
            while (table.rows.length > 1) {
                table.deleteRow(1);
            }

            for (var i = 0; i < devices.length; i++) {
                var row = table.insertRow(i + 1);
                var nameCell = row.insertCell(0);
                var bdaddrCell = row.insertCell(1);
                var statusCell = row.insertCell(2);
                var uuidsCell = row.insertCell(3);
                var connectCell = row.insertCell(4);


                nameCell.innerHTML = devices[i].name;
                bdaddrCell.innerHTML = devices[i].bdaddr;
                uuidsCell.innerHTML = devices[i].UUIDs;
                statusCell.innerHTML = devices[i].connected ? 'Connected' : 'Not Connected';

                var connectButton = document.createElement("button");
                connectButton.innerHTML = "Connect";
                connectButton.addEventListener("click", function () {
                    var bdaddr_ble = this.dataset.bdaddr;
                    var toggleConnectionState = this.dataset.toggleConnectionState;
                    // console.log("Connecting to:______" + i);
                    connectToDevice(bdaddr_ble, toggleConnectionState);
                });
                connectButton.dataset.bdaddr = row.cells[1].innerHTML;
                connectButton.dataset.toggleConnectionState = row.cells[2].innerHTML;
                connectCell.appendChild(connectButton);
            }
        }
    };
    xhttp.open("GET", "http://raspberrypi/cgi-bin/gateway/do_discover_devices.py?scantime=3000", true);
    xhttp.send();
}

function connectToDevice(bdaddr_ble, toggleConnectionState) {
    // console.log("Connecting to: " + bdaddr_ble);
    // console.log("Connecting ?" + toggleConnectionState);
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            // console.log(this.responseText);
            result = JSON.parse(this.responseText);
            message(result_string(result.result));
            if (result.result == 0) {
                var table = document.getElementById("device-table");
                if (toggleConnectionState == "Not Connected") {
                    for (var i = 1; i < table.rows.length; i++) {
                        var bdaddr_cell = table.rows[i].cells[1];
                        if (bdaddr_cell.innerHTML === bdaddr_ble) {
                            var connected_cell = table.rows[i].cells[2];
                            connected_cell.innerHTML = "Connected";
                        }
                    }
                } else {
                    for (var i = 1; i < table.rows.length; i++) {
                        var bdaddr_cell = table.rows[i].cells[1];
                        if (bdaddr_cell.innerHTML === bdaddr_ble) {
                            var connected_cell = table.rows[i].cells[2];
                            connected_cell.innerHTML = "Not Connected";
                        }
                    }
                }
                onServiceDiscovery(bdaddr_ble)
            }
        }
    };

    var args = {};
    args.bdaddr = bdaddr_ble;
    var json = JSON.stringify(args);
    var target = "do_connect.py";
    if (toggleConnectionState != "Not Connected") {
        target = "do_connect.py";
        info = "disconnecting...";
    }
    var info = "connecting..";
    xhttp.open("PUT", "http://raspberrypi/cgi-bin/gateway/" + target, true);
    xhttp.setRequestHeader('Content-type', 'application/json; charset=utf-8');
    xhttp.send(json);
    message(info);
}
function onServiceDiscovery(bdaddr_ble) {
    message("performing service discovery");
    // console.log("TRUE");
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            services = JSON.parse(this.responseText);
            if (services.result !== undefined) {
                message("ERROR " + services.result);
            } else {
                // console.log(services);
                message("");
                var service_count = services.length;
                var table = document.getElementById('service-table');
                var tbody = table.querySelector('tbody');
                while (table.rows.length > 1) {
                    table.deleteRow(1);
                }
                services.forEach(function (service) {
                    var row = tbody.insertRow();
                    var uuidCell = row.insertCell(0);
                    uuidCell.textContent = service.UUID;

                    var handleCell = row.insertCell(1);
                    handleCell.textContent = service.handle;

                    // table of characteristics
                    var charCell = row.insertCell(2);
                    var charTable = document.createElement('table');
                    charTable.className = 'characteristics-table';
                    charTable.setAttribute('border', '1');

                    var charTableHeader = charTable.createTHead();
                    var charTableHeaderRow = charTableHeader.insertRow();
                    var charUUIDHeader = charTableHeaderRow.insertCell(0);
                    charUUIDHeader.textContent = 'UUID';
                    var charNotifyingHeader = charTableHeaderRow.insertCell(1);
                    charNotifyingHeader.textContent = 'Notifying';
                    var charPropsHeader = charTableHeaderRow.insertCell(2);
                    charPropsHeader.textContent = 'Properties';

                    var charTableBody = charTable.createTBody();

                    service.characteristics.forEach(function (characteristic) {
                        var charTableRow = charTableBody.insertRow();
                        var charUUIDCell = charTableRow.insertCell(0);
                        charUUIDCell.textContent = characteristic.UUID;
                        var charNotifyingCell = charTableRow.insertCell(1);
                        charNotifyingCell.textContent = characteristic.notifying ? 'Yes' : 'No';

                        var charActionsCell = charTableRow.insertCell(2);
                        var actions = [];
                        if (characteristic.properties.includes('read')) {
                            var readButton = document.createElement('button');
                            readButton.textContent = 'read';
                            readButton.addEventListener('click', function () {
                                testclick('read', bdaddr_ble, readButton.dataset.characteristic_handle);
                            });
                            readButton.dataset.characteristic_handle = characteristic.handle
                            charActionsCell.appendChild(readButton);
                            actions.push('read');
                        }
                        if (characteristic.properties.includes('write')) {
                            var writeButton = document.createElement('button');
                            writeButton.textContent = 'write';
                            writeButton.addEventListener('click', function () {
                                testclick('write', bdaddr_ble, readButton.dataset.characteristic_handle);
                            });
                            writeButton.dataset.characteristic_handle = characteristic.handle
                            charActionsCell.appendChild(writeButton);
                            
                            actions.push('write');
                        }
                        if (characteristic.properties.includes('notify')) {
                            var notifyButton = document.createElement('button');
                            notifyButton.textContent = 'notify';
                            notifyButton.addEventListener('click', function () {
                                testclick('notify', bdaddr_ble, readButton.dataset.characteristic_handle);
                            });
                            notifyButton.dataset.characteristic_handle = characteristic.handle
                            charActionsCell.appendChild(notifyButton);
                            
                            actions.push('notify');
                        }
                        if (characteristic.properties.includes('indicate')) {
                            var indicateButton = document.createElement('button');
                            indicateButton.textContent = 'indicate';
                            indicateButton.addEventListener('click', function () {
                                testclick('indicate');
                            });
                            charActionsCell.appendChild(indicateButton);
                            actions.push('indicate');
                        }
                    });

                    // Loại bỏ dấu phẩy cuối cùng
                    charCell.appendChild(charTable);
                });

                message("ready");
            }
        }
    };
    xhttp.open("GET", "http://raspberrypi/cgi-bin/gateway/do_service_discovery.py?bdaddr=" + bdaddr_ble, true);
    xhttp.send();
}
function testclick(action, bdaddr, characteristic_handle) {
    if (action == "write") {
        var value_hex = stringToHex(prompt("Enter a value: "));
        writecharacteristic(bdaddr, characteristic_handle,value_hex);
    }
    else if (action == "read") {
        readCharacteristic(bdaddr, characteristic_handle);
    }
    else if (action == "notify" || action == "indicate") {
        console.log(characteristic_handle);
    }
    
}
function notifyCharacteristic(bdaddr, characteristic_handle, signal) {
    var ws
    if (signal == false){
        console.log("enabling...");
        ws = new WebSocket(NOTIFICATIONS_SERVER);
        ws.onerror = function(event) {
            err = "ERROR: Web Socket for notification control and transport is not available";
            message(err);
            console.log(err);
            alert(err);
        };
        ws.onopen = function(e) {
            // enable notifications
            var control_message = {
            "bdaddr" : bdaddr,
            "handle" : characteristic_handle,
            "command" : NOTIFICATIONS_ON
            };
            console.log(JSON.stringify(control_message));
            ws.send(JSON.stringify(control_message));
        }
        ws.onmessage = function(event) {
            result = JSON.parse(event.data);
            console.log("onmessage: "+JSON.stringify(result));
            if (result.value !== undefined) {
            var attribute_indices = handle_map.get(result.handle);
            this_service_inx = attribute_indices[0];
            this_characteristic_inx = attribute_indices[1];
            characteristic = services[this_service_inx].characteristics[this_characteristic_inx];
            // convert to byte array from hex format and store
            characteristic.value = hexToBytes(result.value);
            var display_value = convertFromHex(result.value,characteristic.format);
            element("char_val_"+this_service_inx+"_"+this_characteristic_inx).value = display_value;
            } else if (result.result == 0) {
            var attribute_indices = handle_map.get(result.handle);
            this_service_inx = attribute_indices[0];
            this_characteristic_inx = attribute_indices[1];
            characteristic = services[this_service_inx].characteristics[this_characteristic_inx];
            if (characteristic.notifying == false) {
                console.log("notifications enabled OK");
                characteristic.notifying = true;
                } else {
                console.log("notifications disabled OK");
                characteristic.notifying = false;
                ws.close();
                ws_map.delete(handle);
            }
            } else {
                        message(error[result.result]);
            }
        };
        ws_map.set(handle,ws);
    }
    else {
        console.log("disabling...");
        ws = ws_map.get(handle);
        console.log(ws);
        var control_message = {
            "bdaddr" : selected_device.bdaddr,
            "handle" : characteristic.handle,
            "command" : NOTIFICATIONS_OFF
        };
        console.log(JSON.stringify(control_message));
        ws.send(JSON.stringify(control_message));
    }
}


function readCharacteristic(bdaddr, characteristic_handle) {
    console.log(bdaddr, characteristic_handle);
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            console.log(this.responseText);
            result = JSON.parse(this.responseText);
            // convert to byte array and store in the characteristic object
            message(result_string(result.result));
            if (result.result == 0) {
                alert("READING Characteristic -------------------- Value: " + hexToChar(result.value))
            }
        }
    };
    xhttp.open("GET", "http://raspberrypi/cgi-bin/gateway/do_read_characteristic.py" + "?bdaddr=" + bdaddr + "&handle=" + characteristic_handle, true);
    xhttp.send();
}

function writecharacteristic(bdaddr, characteristic_handle,value_hex){
    console.log(bdaddr, characteristic_handle);
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            console.log(this.responseText);
            result = JSON.parse(this.responseText);
            // convert to byte array and store in the characteristic object
            message(result_string(result.result));
            if (result.result == 0) {
                alert("Writing Characteristic OK")
            }
        }
    };
    var args = {};
    args.bdaddr = bdaddr;
    args.handle = characteristic_handle;
    args.value = value_hex
    var json = JSON.stringify(args);
    console.log(json);
    xhttp.open("PUT", "http://raspberrypi/cgi-bin/gateway/do_write_characteristic.py", true);
    xhttp.setRequestHeader('Content-type', 'application/json; charset=utf-8');
    xhttp.send(json);
}


function message(text) {
    element('message').innerHTML = text;
    var xhttp = new XMLHttpRequest();
}
function element(id) {
    return document.getElementById(id);
}
function result_string(result) {
    if (result == 0) {
        return "OK";
    } else {
        return "ERROR " + result;
    }
}
function hexToChar(hex) {
    var charStr = '';
    for (var i = 0; i < hex.length; i += 2) {
      var charCode = parseInt(hex.substr(i, 2), 16);
      charStr += String.fromCharCode(charCode);
    }
    return charStr;
  }
function stringToHex(input) {
    let hex = '';
    for (let i = 0; i < input.length; i++) {
        let charCode = input.charCodeAt(i).toString(16);
        hex += charCode.length === 1 ? '0' + charCode : charCode;
    }
    return hex;
}