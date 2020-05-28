const fs = require('fs-extra');
const path = require('path');
const separator = '\n        ';
const STM32_FM_REPO = 'cube-repo';

function removeComments(lines) {
    let result = [].concat(lines);
    for (let i = 0; i < lines.length; i++) {
        let commentRegExp = /(?<!\^)(#.*$)/
        let escapeRegExp = /\^#/g;

        let line = lines[i];
        line = line.replace(commentRegExp, '');
        line = line.replace(escapeRegExp, '#');
        result[i] = line.replace(commentRegExp, '');
    }

    return result;
}

function removeEmptyLines(array) {
    let result = [];
    let regexp = /^\s*$/;
    for (let i = 0; i < array.length; i++) {
        let line = array[i];
        let m = line.match(regexp);
        if (!m) {
            result.push(line);
        }
    }
    return result;
}

function joinLines(array) {
    let doJoin = false;
    let result = [];
    for (let i = 0; i < array.length; i++) {
        let line = array[i];
        let m = line.match(/(.*)\\\s*$/);
        if (m) {
            line = m[1];
        }

        if (doJoin) {
            result[result.length - 1] += line;
        } else {
            result.push(line);
        }

        doJoin = !!m;
    }
    return result;
}

function collectVariables(array) {
    let map = {};
    for (let i = 0; i < array.length; i++) {
        let line = array[i];
        let m = line.match(/^\s*(\w+)\s*=\s*(.*)$/);
        if (m) {
            map[m[1]] = m[2];
        }
    }
    return map;
}

function calculateVaribles(map) {
    let result = {};
    let queue = [].concat(Object.keys(map));
    while (queue.length > 0) {
        let variable = queue.shift();
        let expr = map[variable];
        let computed = expr;
        let processed = true;
        let m;
        const regex = /\$\(([\w-]+)\)/gm;
        while (processed && ((m = regex.exec(expr)) !== null)) {
            if (m.index === regex.lastIndex) {
                regex.lastIndex++;
            }

            let name = m[1];
            let initialized = map[name];
            let value = result[m[1]];
            if (!initialized) {
                value = '';
            }
            if (value !== undefined) {
                computed = computed.replace(m[0], value);
            }
            processed = processed && value !== undefined;
        }

        if (processed) {
            result[variable] = computed;
        } else {
            queue.push(variable);
        }
    }

    return result;
}

function initSources(result, projectPath) {
    let relPath = path.relative(__dirname, projectPath);
    let src = result['C_SOURCES'];
    let array = src.split(' ').filter((v) => v.length !== 0);
    array.push(result['ASM_SOURCES']);
    array = array.map((v) => {
        let regexp = /^.*STM32Cube\/Repository\//;
        if (v.match(regexp)) {
            return v.replace(/^.*STM32Cube\/Repository\//, `${STM32_FM_REPO}/`);
        }
        return path.join(relPath, v);
    });
    array = array.filter(function (item, pos) {
        return array.indexOf(item) === pos;
    }).sort();

    let list = array.join(separator);
    let target = result['TARGET'].toUpperCase() + '_SRC';
    return `set(${target} ${separator}${list})`;
}

function initIncludesDir(result, projectPath) {
    let relPath = path.relative(__dirname, projectPath);
    let includes = result['C_INCLUDES'];
    let array = includes.split(' ').filter((v) => v.length !== 0);
    array = array.map((v) => v.replace(/^\s*-I/, ''));
    array = array.map((v) => {
        let regexp = /^.*STM32Cube\/Repository\//;
        if (v.match(regexp)) {
            return v.replace(/^.*STM32Cube\/Repository\//, `${STM32_FM_REPO}/`);
        }
        return path.join(relPath, v);
    });
    array = array.filter(function (item, pos) {
        return array.indexOf(item) === pos;
    }).sort();

    let list = array.join(separator);
    let target = result['TARGET'].toUpperCase() + '_INC';
    return `set(${target} ${separator}${list})`;
}

function initCompileDefinitions(result) {
    let target = result['TARGET'].toUpperCase() + '_DEFS';
    let defs = result['C_DEFS'];

    return `set(${target} ${defs})`;
}

function initCompileOptions(result) {
    let target = result['TARGET'].toUpperCase() + '_OPT_MCU';
    let defs = result['MCU'].trim();

    return `set(${target} ${defs})`;
}

function initLinker(result, projectPath) {
    let target = result['TARGET'].toUpperCase() + '_LD';
    let LDSCRIPT = result['LDSCRIPT'];
    let relPath = path.relative(__dirname, projectPath);

    return `set(${target} ${relPath}/${LDSCRIPT})`;
}

function processMakeSettings(lines, projectPath) {
    let array = removeComments(lines);
    array = removeEmptyLines(array);
    array = joinLines(array);

    let map = collectVariables(array);
    map = calculateVaribles(map);

    return [
        '',
        `# STM32CubeMX project variables for ${projectPath}`,
        initSources(map, projectPath),
        initIncludesDir(map, projectPath),
        initCompileDefinitions(map, projectPath),
        initCompileOptions(map, projectPath),
        initLinker(map, projectPath)
    ]
}

function processMxProject(projectPath) {
    let mxProject = path.join(projectPath, 'Makefile');

    if (fs.existsSync(mxProject)) {
        let content = fs.readFileSync(mxProject, "utf8");
        let lines = content.split(/[\r\n]/);
        return processMakeSettings(lines, projectPath);
    } else {
        console.error(`Makefile not found ${projectPath}`)
    }

    return [];
}

const mxProjectFolder = 'cubemx-proj'
let array = [];

// array = processMxProject(path.join(mxProjectFolder, 'stm32f103_blue_pill'));
fs.readdirSync(mxProjectFolder).forEach(file => {
    try {
        let a = processMxProject(path.join(mxProjectFolder, file));
        array = array.concat(a);
    } catch (e) {
        console.error(e);
    }
});

processMxProject(path.join(mxProjectFolder, 'stm32f103_blue_pill'));


let content = fs.readFileSync('CMakeLists.txt', "utf8");
let lines = content.split(/\n/);
let startPos = lines.indexOf('# Begin cubemx-proj variables');
let endPos = lines.indexOf('# End cubemx-proj variables')

if (startPos !== -1 && endPos !== -1) {
    lines.splice(startPos,
        endPos - startPos + 1,
        '# Begin cubemx-proj variables',
        array.join('\n'),
        '# End cubemx-proj variables');
    content = lines.join('\n');
    fs.writeFileSync('CMakeLists.txt', content, 'utf8');
}