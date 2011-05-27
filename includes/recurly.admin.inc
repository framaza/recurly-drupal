<?php

/**
 * @file
 * Recurly settings forms and administration page callbacks.
 */


/**
 * Returns the site-wide Recurly settings form.
 */
function recurly_settings_form($form, &$form_state) {
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default account settings'),
    '#description' => t('Configure these settings based on your Company Settings and API Credentials settings in the Recurly administration interface.'),
  );
  $form['account']['recurly_api_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => variable_get('recurly_api_username', ''),
  );
  $form['account']['recurly_api_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => variable_get('recurly_api_password', ''),
  );
  $form['account']['recurly_subdomain'] = array(
    '#type' => 'textfield',
    '#title' => t('Subdomain'),
    '#description' => t("The subdomain of your account excluding the -test suffix if still in Sandbox."),
    '#default_value' => variable_get('recurly_subdomain', ''),
  );
  $form['account']['recurly_environment'] = array(
    '#type' => 'radios',
    '#title' => t('Environment'),
    '#description' => t('Select the environment that matches your account status.'),
    '#options' => array(
      'sandbox' => t('Sandbox'),
      'production' => t('Production'),
    ),
    '#default_value' => variable_get('recurly_environment', 'sandbox'),
  );

  return system_settings_form($form);
}
