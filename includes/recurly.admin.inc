<?php

/**
 * @file
 * Recurly settings forms and administration page callbacks.
 */


/**
 * Returns the site-wide Recurly settings form.
 */
function recurly_settings_form($form, &$form_state) {
  $form['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default API settings'),
    '#description' => t('Configure these settings based on your API Credentials settings in the Recurly administration interface.'),
  );
  $form['api']['recurly_api_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => variable_get('recurly_api_username', ''),
  );
  $form['api']['recurly_api_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => variable_get('recurly_api_password', ''),
  );
  $form['api']['recurly_api_endpoint'] = array(
    '#type' => 'radios',
    '#title' => t('Endpoint'),
    '#description' => t('Select the endpoint that matches your account status.'),
    '#options' => array(
      'sandbox' => t('Sandbox'),
      'production' => t('Production'),
    ),
    '#default_value' => variable_get('recurly_api_endpoint', 'sandbox'),
  );

  return system_settings_form($form);
}
