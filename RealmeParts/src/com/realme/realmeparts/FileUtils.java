/*
 * Copyright (C) 2018-2019 The Xiaomi-SDM660 Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License
 */

package com.realme.realmeparts;

import android.os.SystemProperties;
import android.util.Log;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;

public class FileUtils {
    private static final String TAG = "FileUtils";

    /**
     * Reads the first line of text from the given file.
     * Reference {@link BufferedReader#readLine()} for clarification on what a line is
     *
     * @return the read line contents, or null on failure
     */
    public static String readOneLine(String fileName) {
        String line = null;
        BufferedReader reader = null;

        try {
            reader = new BufferedReader(new FileReader(fileName), 512);
            line = reader.readLine();
        } catch (FileNotFoundException e) {
            Log.w(TAG, "No such file " + fileName + " for reading", e);
        } catch (IOException e) {
            Log.e(TAG, "Could not read from file " + fileName, e);
        } finally {
            try {
                if (reader != null) {
                    reader.close();
                }
            } catch (IOException e) {
                // Ignored, not much we can do anyway
            }
        }

        return line;
    }

    static boolean fileWritable(String filename) {
        return fileExists(filename) && new File(filename).canWrite();
    }

    private static boolean fileExists(String filename) {
        if (filename == null) {
            return false;
        }
        return new File(filename).exists();
    }

    public static void setValue(String path, Boolean value) {
        if (fileWritable(path)) {
            if (path == null) {
                return;
            }
            try {
                FileOutputStream fos = new FileOutputStream(new File(path));
                fos.write((value ? "1" : "0").getBytes());
                fos.flush();
                fos.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public static void setValue(String path, int value) {
        if (fileWritable(path)) {
            if (path == null) {
                return;
            }
            try {
                FileOutputStream fos = new FileOutputStream(new File(path));
                fos.write(Integer.toString(value).getBytes());
                fos.flush();
                fos.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    static void setValue(String path, double value) {
        if (fileWritable(path)) {
            if (path == null) {
                return;
            }
            try {
                FileOutputStream fos = new FileOutputStream(new File(path));
                fos.write(Long.toString(Math.round(value)).getBytes());
                fos.flush();
                fos.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public static void setValue(String path, String value) {
        if (fileWritable(path)) {
            if (path == null) {
                return;
            }
            try {
                FileOutputStream fos = new FileOutputStream(new File(path));
                fos.write(value.getBytes());
                fos.flush();
                fos.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    static String getValue(String filename) {
        if (filename == null) {
            return null;
        }
        String line;
        try (BufferedReader br = new BufferedReader(new FileReader(filename), 1024)) {
            line = br.readLine();
        } catch (IOException e) {
            return null;
        }
        // ignore
        return line;
    }

    static void setProp(String prop, boolean value) {
        if (value) {
            SystemProperties.set(prop, "1");
        } else {
            SystemProperties.set(prop, "0");
        }
    }

    static String readLine(String filename) {
        if (filename == null) {
            return null;
        }
        BufferedReader br = null;
        String line;
        try {
            br = new BufferedReader(new FileReader(filename), 1024);
            line = br.readLine();
        } catch (IOException e) {
            return null;
        } finally {
            if (br != null) {
                try {
                    br.close();
                } catch (IOException e) {
                    // ignore
                }
            }
        }
        return line;
    }

    public static boolean getFileValueAsBoolean(String filename, boolean defValue) {
        String fileValue = readLine(filename);
        if (fileValue != null) {
            return !fileValue.equals("0");
        }
        return defValue;
    }
}
